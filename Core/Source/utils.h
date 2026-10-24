#pragma once
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <utility>
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
inline UIntHash hashUIntPair(const std::pair<unsigned int, unsigned int> &p) {
  auto numOfBits = sizeof(unsigned int) * CHAR_BIT;
  return (static_cast<UIntHash>(p.first) << numOfBits) |
         static_cast<UIntHash>(p.second);
}

} // namespace Core
