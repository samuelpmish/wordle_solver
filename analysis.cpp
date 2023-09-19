#include "wordle_tools.hpp"

#include <unordered_map>

struct array_hasher {
  template<size_t n>
  std::size_t operator()(const std::array< Clue, n > & arr) const {
    uint64_t seed = 0;
    for(const auto elem : arr) {
      seed ^= std::hash<uint64_t>()(elem) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }
    return seed;
  }
};

using clue_count = std::unordered_map< std::array<Clue, 5>, int, array_hasher >;

clue_count get_counts(const std::vector< Word > & words, Word guess) {
  clue_count counts;
  for (auto w : words) counts[get_clues(guess, w)]++;
  return counts;
}

int main() {

  Word guess = "aloes";

  clue_count counts = get_counts(all_answers, guess);

  for (auto & [k, v] : counts) {
    std::cout << k << ": " << v << std::endl;
  }

}