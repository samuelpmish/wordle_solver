#pragma once

#include "word.hpp"

extern std::vector < Word > all_words;
extern std::vector < Word > all_answers;

Word random(const std::vector< Word > & words);

enum Clue { GRAY, YELLOW, GREEN };

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

State combine(State a, State b);

void print(State s);

std::vector< Word > select(const std::vector < Word > & words, const State & s);

size_t num_remaining_words(const std::vector < Word > & words, const State & s);

std::array<Clue, 5> get_clues(Word answer, Word guess);

auto check(Word answer, Word guess);

auto best_guess_brute_force(const std::vector < Word > & possible_words, State clues);

int wordle_solve(Word answer, bool debug_print = false);

void delete_line();

void print(std::array< Clue, 5 > clues, Word guess);

std::ostream& operator<<(std::ostream & out, std::array< Clue, 5 > clues);