#include "utf8Str.h"
#include <bit>
#include <bitset>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
namespace Core {

rune Utf8String::operator[](unsigned int index) const {
  std::cerr << "size of text: " << m_text.size() << "\n";
  std::cerr << "size of utf8: " << m_length << "\n";
  unsigned int pos = -1;
  rune out = 0;
  for (unsigned int i = 0; i < m_text.size(); i++) {
    unsigned char s = m_text[i];

    // check if first 1 is followed by 0 and ignore those only count that
    // arent followed by 0
    // 3 = 00000011
    // shifted to 6 is 11000000

    unsigned char maskFor1countage = 3 << (sizeof(unsigned char) * 8 - 2);

    // 1 shifted to 7 is 10000000

    if ((s & maskFor1countage) != (1 << (sizeof(unsigned char) * 8 - 1))) {
      pos += 1;
    }

    if (pos != index)
      continue;

    // count first 1 bits
    auto numOf1 = std::countl_one(s);
    unsigned char firstByte = m_text[i];
    std::cerr << "firstByte: " << std::bitset<8>(firstByte) << "\n";
    // clear first n bits that represent how many codepoints to use plus one
    // extra bit
    std::cerr << "pos: " << pos << " numOf1: " << numOf1 << " index: " << index
              << "\n";
    firstByte &= 0b11111111 >> (numOf1 + 1);
    std::cerr << "firstByte: " << std::bitset<8>(firstByte) << "\n";
    rune mask = rune(firstByte);
    std::cerr << "mask: " << std::bitset<32>(mask) << "\n";

    // (num1 -1) cuz for last byte no left shift, so starting from 0 to numOf1-1
    // and shift by 6 cuz first 2 bits of following bytes is ignored

    if (numOf1 > 1)
      mask <<= 6 * (numOf1 - 1);

    out |= mask;
    std::cerr << "out: " << std::bitset<32>(out) << "\n";
    if (numOf1 > 1) {
      // reverse loop throgh characters from first valid rune to its last
      // code point
      for (unsigned int j = i + numOf1 - 1; j > i; j--) {
        std::cerr << "this runs twice? numOf1: " << numOf1 << "\n";
        if (j > m_text.size()) {
          exit(-1);
        }
        unsigned char byte = m_text[j];
        std::cerr << "text index: " << j << " bitset: " << std::bitset<8>(byte)
                  << "\n\n";
        // cloar first 2 bit
        byte &= 0b00111111;
        rune mask = rune(byte);
        // left shift the mask based on where i want the output's bits to be
        // replaced, by 6 cuz same reason as before
        mask <<= ((i + numOf1 - 1) - j) * 6;
        std::cerr << "mask: " << std::bitset<32>(mask) << "\n";
        // add the byte to output rune
        out |= mask;
        std::cerr << "out: " << std::bitset<32>(mask) << "\n";
      }
    }

    return out;
  }
  throw std::runtime_error("index out of range");
}

} // namespace Core
