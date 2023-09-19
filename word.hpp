#include <string>
#include <iostream>
#include <algorithm> // for std::copy

constexpr size_t word_length = 5;
struct Word {
  Word() : data{} {}
  Word(std::string str) { 
    if (str.size() != word_length) { exit(1); }
    std::copy(str.begin(), str.end(), data); 
  }
  Word(const char tmp[6]) { std::copy(tmp, tmp+word_length, data); }
  char data[word_length]; 

  bool contains(char c) const {
    for (int i = 0; i < word_length; i++) {
      if (c == data[i]) { return true; }
    }
    return false;
  }

  int find(char c) const {
    for (int i = 0; i < word_length; i++) {
      if (c == data[i]) { return i; }
    }
    return -1;
  }

  char & operator[](int i) { return data[i]; }
  const char & operator[](int i) const { return data[i]; }
};

inline uint32_t letter_mask(Word w) {
  constexpr uint32_t one = 1;
  uint32_t mask{}; 
  for (auto c : w.data) mask |= (one << (c - 'a')); 
  return mask;
}

inline uint32_t letter_mask(char c) { 
  if (c == '?') return uint32_t(0);
  else return uint32_t(1) << (c - 'a'); 
}

inline std::ostream & operator<<(std::ostream & out, Word w) {
  for (auto c : w.data) {
    out << c;
  } 
  return out;
}
