#pragma once
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
namespace Core {
inline bool nearZero(float x) { return std::abs(x) < 0.0001f; }

typedef uint64_t UIntHash;

// TODO: Make a better hash function than this

/**
 * @brief This is not a secure hash, it just takes two 32 bit input and spits
 * out an 64 bit output by appending second input behind first thus making it
 * unique for each pair combination
 *
 * @param p a pair of unsigned ints
 * @return uint64_t
 */
inline UIntHash hashUIntPair(const unsigned first, unsigned int second) {
  auto numOfBits = sizeof(unsigned int) * CHAR_BIT;
  return (static_cast<UIntHash>(first) << numOfBits) |
         static_cast<UIntHash>(second);
}
template <typename... Delimiters>
  requires(std::is_convertible_v<Delimiters, char> && ...)
inline std::vector<std::string> splitString(const std::string &s,
                                            const Delimiters &...delimiter) {
  std::vector<std::string> retVal;
  std::string temp = "";
  for (auto const &str : s) {
    if (((str == delimiter) || ...)) {
      retVal.push_back(temp);
      temp = "";
      continue;
    }
    temp.push_back(str);
  }
  retVal.push_back(temp);
  return retVal;
}

} // namespace Core
