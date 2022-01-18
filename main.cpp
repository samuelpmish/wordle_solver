#include <array>
#include <set>
#include <iostream>
#include <algorithm>

#include "small_array.h"
#include "word_list.h"

using Clue = std::pair< int, char >;

struct State {
  std::set < Clue > matched; 
  std::set < Clue > misplaced; 
  uint32_t unused; 

  // check if word satisfies the given information about 
  // which characters are matched, misplaced or unused
  bool is_consistent_with(Word word) const {

    uint32_t mask = letter_mask(word);

    // the given word must match certain letters at specific locations
    for (auto [i, c] : matched) { 
      if (word[i] != c) return false; 
    }

    // the given word must not contain certain letters
    if (mask & unused) return false;

    // the given word must contain certain letters, but not in the given location
    for (auto [i, c] : misplaced) { 
      if (word[i] == c) return false; 
      if (!word.contains(c)) return false; 
    }

    return true;
  }
};

// set union
template < typename T >
std::set< T > operator+(const std::set< T > & a, const std::set < T > & b) {
  std::set<T> result = a;
  result.insert(b.begin(), b.end());
  return result;
}

// set difference
template < typename T >
std::set< T > operator-(const std::set< T > & a, const std::set < T > & b) {
  std::set<T> result;
  std::set_difference(
      a.begin(), a.end(),
      b.begin(), b.end(),
      std::inserter(result, result.end()));
  return result;
}

State combine(State a, State b) {
  auto matched = a.matched + b.matched;
  return {
    matched,
    (a.misplaced + b.misplaced) - matched,
    a.unused | b.unused
  };
}

void print(State s) {
  std::cout << "matched: ";
  for (auto [i, c] : s.matched) {
    std::cout << '{' << c << ", " << i << "} ";
  }
  std::cout << std::endl;

  std::cout << "misplaced: ";
  for (auto [i, c] : s.misplaced) {
    std::cout << '{' << c << ", " << i << "} ";
  }
  std::cout << std::endl;

  std::cout << "unused: ";
  for (int i = 0; i < 26; i++) {
    if (s.unused & (1 << i)) std::cout << 'a' + i << " ";
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
    if (matching[i]) feedback.matched.insert({i, answer[i]});
  }

  // then get all the unused / misplaced letters
  for (int i = 0; i < word_length; i++) {
    int location = answer.find(guess[i]);
    if (location == -1) {
      feedback.unused |= 1 << (guess[i] - 'a');
    } else {
      if (!matching[location]) {
        feedback.misplaced.insert({i, guess[i]});
      }
    }
  }

  return feedback;
}

float new_information(State clues, Word word, float (&weights)[word_length][26]) {
  std::set<char> already_used{};

  float score = 0;
  for (int i = 0; i < word_length; i++) {
    char c = word[i];
    if (already_used.count(c) == 0) {
      score += weights[i][c - 'a'];
      already_used.insert(c);
    }
  }

  return score;
}

auto best_guess(std::vector < Word > possible_words, State clues) {

  bool already_matched[word_length]{};
  for (auto [i, c] : clues.matched) { already_matched[i] = true; }

  const float delta = 1.0 / possible_words.size();

  float weights[word_length][26] {};
  for (auto word : possible_words) {
    for (int i = 0; i < word_length; i++) {
      if (already_matched[i]) continue;
      weights[i][word[i] - 'a'] += delta;
      for (int j = 0; j < word_length; j++) {
        weights[j][word[i] - 'a'] += 0.25 * delta;
      }
    }
  }

  for (auto [i, c] : clues.matched) { weights[i][c - 'a'] = 0.0; }
  for (auto [i, c] : clues.misplaced) { weights[i][c - 'a'] = 0.0; }

  float best_score = -1.0;
  Word best_word;

  for (auto word : five_letter_words) {
    float score = new_information(clues, word, weights);
    if (best_score < score) {
      best_score = score;
      best_word = word;
    }
  }

  return best_word;
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

#if 1
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
#else

  State clues {
    {{0, 'a'}, {3, 'e'}, {4, 'y'}},
    {},
    {'d', 'i', 'r', 't', 'm', 'p', 'l', 'o', 's'}
  };

  auto possible_words = select(five_letter_words, clues);

  std::cout << possible_words.size() << " remaining words: " << std::endl;
  int count = 0;
  for (auto word : possible_words) {
    std::cout << word << ' ';
    if (++count % 16 == 0) std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << std::endl;
  
  std::cout << best_guess_brute_force(possible_words, clues) << std::endl;
  

#endif

}
