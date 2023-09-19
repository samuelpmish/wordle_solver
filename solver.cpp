#include <array>
#include <vector>
#include <iostream>
#include <algorithm>

#include "wordle_tools.hpp"

bool hard_mode = true;

int main(int argc, char * argv[]) {

  Word answer = random(all_answers);

  int num_guesses = 0;

  while (num_guesses < 6) {
    std::string guess;
    std::getline(std::cin, guess);
    delete_line();
    if (guess.size() == word_length) {
      auto clues = get_clues(answer, guess);
      print(clues, guess);
      num_guesses++;

      if (clues == std::array{GREEN, GREEN, GREEN, GREEN, GREEN}) break;
    }
  }

  if (num_guesses < 6) {
    std::cout << "🎉🎉🎉🎉🎉" << std::endl;
  } else {
    std::cout << "too bad, the answer was: " << answer << std::endl;
  }


#if 0
  auto possible_words = all_words;

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
    for (auto word : all_words) {
      counts[wordle_solve(word)]++;
    }

    for (int i = 0; i < 10; i++) {
      std::cout << i << ": " << counts[i] << std::endl;
    }
  }
#endif

}
