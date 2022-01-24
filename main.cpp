#include <array>
#include <vector>
#include <iostream>
#include <algorithm>

#include "word.h"

extern std::vector < Word > five_letter_words;

struct State {

  char matched[word_length]; 
  uint32_t misplaced[word_length];
  uint32_t unused; 
  uint32_t used;

  // '?' here will denote we haven't found a match
  State() : matched{'?', '?', '?', '?', '?'}, misplaced{}, unused{}, used{}{}

  // check if word satisfies the given information about 
  // which characters are matched, misplaced or unused
  bool is_consistent_with(Word word) const {

    uint32_t mask = letter_mask(word);

    // the given word must contain certain letters
    if ((mask & used) != used) return false;

    // and not contain others
    if (mask & unused) return false;

    // and also satisfy certain matching conditions
    for (int i = 0; i < word_length; i++) {
      char c = word[i];

      // matched letters must be in certain locations
      if (matched[i] != '?' && c != matched[i]) return false; 

      // misplaced letters must not be in certain locations
      if (letter_mask(c) & misplaced[i]) return false; 
    }

    return true;
  }
};

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

auto select(const std::vector < Word > & words, const State & s) {
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

  for (auto word : five_letter_words) {
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

int wordle_solve(Word answer, bool debug_print = false) {

  State clues{};

  auto possible_words = five_letter_words;

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

int main(int argc, char * argv[]) {

  auto possible_words = five_letter_words;

  State clues{};

  if (argc == 2) { 
    std::string answer = argv[1];
    if (answer.size() != 5) {
      std::cout << "must enter a 5-letter word" << std::endl;
      exit(1);
    }
    bool debug_print;
    wordle_solve(answer, debug_print = true);
  } else {

    int counts[10]{};
    for (auto word : five_letter_words) {
      counts[wordle_solve(word)]++;
    }

    for (int i = 0; i < 10; i++) {
      std::cout << i << ": " << counts[i] << std::endl;
    }
  }

}
