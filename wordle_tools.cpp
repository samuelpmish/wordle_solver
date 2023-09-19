#include "wordle_tools.hpp"

#include "color.hpp"

#include <random>

Word random(const std::vector< Word > & words) {
  static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  static std::default_random_engine generator(seed);
  static std::uniform_int_distribution<int> distribution(0, words.size() - 1);
  return words[distribution(generator)];
}

State combine(State a, State b) {

  State combined{};

  for (int i = 0; i < word_length; i++) {

    // matched will be '?' (ASCII code: 63) when unknown, 
    // and a lower case character (ASCII codes: 97-122) when known. 
    // So, taking the max will select the known value, 
    // if it exists, or leave a '?' otherwise
    combined.matched[i] = std::max(a.matched[i], b.matched[i]);

    uint32_t mask = letter_mask(combined.matched[i]);

    combined.misplaced[i] = (a.misplaced[i] | b.misplaced[i]) & ~mask;

    combined.used |= combined.misplaced[i] | mask;

  }

  combined.unused = a.unused | b.unused; 

  return combined;
}

void print(State s) {
  std::cout << "matched: ";
  for (int i = 0; i < word_length; i++) {
    if (s.matched[i] != '?') {
      std::cout << '{' << s.matched[i] << ", " << i << "} ";
    }
  }
  std::cout << std::endl;

  std::cout << "misplaced: ";
  for (int i = 0; i < word_length; i++) {
    auto mask = s.misplaced[i];
    for (int j = 0; j < 26; j++) {
      if (mask & (uint32_t(1) << j)) {
        std::cout << '{' << char('a' + j) << ", " << i << "} ";
      }
    }
  }
  std::cout << std::endl;

  std::cout << "unused: ";
  for (int i = 0; i < 26; i++) {
    if (s.unused & (uint32_t(1) << i)) std::cout << char('a' + i) << " ";
  }
  std::cout << std::endl;
}

std::vector< Word > select(const std::vector < Word > & words, const State & s) {
  std::vector < Word > filtered;
  for (auto & word : words) {
    if (s.is_consistent_with(word)) {
      filtered.push_back(word);
    }
  } 
  return filtered;
}

size_t num_remaining_words(const std::vector < Word > & words, const State & s) {
  size_t remaining = 0;
  for (auto & word : words) { remaining += s.is_consistent_with(word); } 
  return remaining;
}

std::array<Clue, 5> get_clues(Word answer, Word guess) {
  std::array<Clue, 5> output;

  for (int i = 0; i < word_length; i++) {
    if (guess[i] == answer[i]) {
      output[i] = GREEN;
    } else {
      bool misplaced = false;
      for (int j = 0; j < word_length; j++) {
        if (guess[i] == answer[j] && guess[j] != answer[j]) {
          misplaced = true;
          break;
        }
      }
      output[i] = misplaced ? YELLOW : GRAY;
    }
  }

  return output;
}

auto check(Word answer, Word guess) {
  State feedback{};

  bool matching[word_length];

  // first get all the matched letters
  for (int i = 0; i < word_length; i++) {
    matching[i] = (answer[i] == guess[i]);
    if (matching[i]) feedback.matched[i] = answer[i];
  }

  // then get all the unused / misplaced letters
  for (int i = 0; i < word_length; i++) {
    int location = answer.find(guess[i]);
    if (location == -1) {
      feedback.unused |= letter_mask(guess[i]);
    } else {
      if (!matching[location]) {
        feedback.misplaced[i] |= letter_mask(guess[i]);
      }
    }
  }

  return feedback;
}

auto best_guess_brute_force(const std::vector < Word > & possible_words, State clues) {

  float fewest_remaining = possible_words.size();
  Word best_word;

  for (auto word : all_words) {
    size_t remaining = 0;
    for (auto answer : possible_words) {
      auto new_clues = combine(clues, check(answer, word));
      remaining = std::max(remaining, num_remaining_words(possible_words, new_clues));
    }

    if (remaining < fewest_remaining) {
      fewest_remaining = remaining;
      best_word = word;
    }
  }

  return best_word;

}

int wordle_solve(Word answer, bool debug_print) {

  State clues{};

  auto possible_words = all_words;

  Word guess = "aloes";

  for (int i = 1; i < 10; i++) {

    std::cout << "guess " << i << ": " << guess << std::endl;
  
    clues = combine(clues, check(answer, guess));

    possible_words = select(possible_words, clues);

    if (debug_print) { 
      print(clues);

      std::cout << possible_words.size() << " remaining words: " << std::endl;
      int count = 0;
      for (auto word : possible_words) {
        std::cout << word << ' ';
        if (++count % 16 == 0) std::cout << std::endl;
      }
      std::cout << std::endl;
      std::cout << std::endl;
    }

    if (possible_words.size() == 1) { 
      std::cout << "found " << possible_words[0] << " in " << i+1 << " guess(es)" << std::endl;
      return i; 
    } else {
      guess = best_guess_brute_force(possible_words, clues);
    }

  } 

  return -1;

}

void delete_line() {
  std::cout << "\x1b[1A\x1b[2K" << std::flush;
}

void print(std::array< Clue, 5 > clues, Word guess) {
  for (int i = 0; i < word_length; i++) {
    std::cout << colors::bold << colors::grey;
    if (clues[i] == GRAY) std::cout << colors::on_bright_grey;
    if (clues[i] == YELLOW) std::cout << colors::on_yellow;
    if (clues[i] == GREEN) std::cout << colors::on_green;
    std::cout << guess[i];
  }
  std::cout << colors::reset << std::endl;
};

std::ostream& operator<<(std::ostream & out, std::array< Clue, 5 > clues) {
  for (int i = 0; i < word_length; i++) {
    if (clues[i] == GRAY) out << "⬜";
    if (clues[i] == YELLOW) out << "🟨";
    if (clues[i] == GREEN) out << "🟩";
  }
  return out;
};