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

}
