#include <bitset>
#include <cstddef>
#include <iostream>
#include <string>
namespace Core {
typedef char32_t rune;
// FIXME: doesn't handle invalid utf8 strings and need to make it work properly
class Utf8String {
public:
  Utf8String(const std::string &str) : m_text(str) {
    m_length = 0;
    std::cerr << "inside Constructor loop" << "\n";
    for (unsigned char s : str) {
      // check if first 1 is followed by 0 and ignore those only count that
      // arent followed by 0
      // 3 = 00000011
      // shifted to 6 is 11000000

      auto mask = 3 << (sizeof(unsigned char) * 8 - 2);

      // 1 shifted to 7 is 10000000
      std::cerr << std::bitset<8>(s) << "\n";

      if ((s & mask) != 1 << (sizeof(unsigned char) * 8 - 1)) {
        m_length += 1;
      }
    }
    std::cerr << "outside Constructor loop" << "\n";
  }

  rune operator[](unsigned int index) const;

  const std::string &str() { return m_text; }

  unsigned int length() { return m_length; }

private:
  std::string m_text;
  unsigned int m_length;
};

inline Utf8String operator""_utf8(const char *str, std::size_t length) {
  std::string text = std::string(str, length);
  return Utf8String(text);
}

} // namespace Core
