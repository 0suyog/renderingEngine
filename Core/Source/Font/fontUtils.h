#pragma once

#include "Font/glyph.h"
#include "coreUtils.h"
#include <sstream>
#include <string>
namespace Core {
inline std::string encodeGlyph(const glyph &g) {
  std::string str =
      std::to_string(g.id) + ";" + std::to_string(g.atlasTop) + ";" +
      std::to_string(g.atlasLeft) + ";" + std::to_string(g.atlasRight) + ";" +
      std::to_string(g.atlasBottom) + ";" + std::to_string(g.left) + ";" +
      std::to_string(g.bottom) + ";" + std::to_string(g.right) + ";" +
      std::to_string(g.top) + "\n";
  return str;
}

// TODO: Maybe validate if the string is a valid encoded glyph string before
// encoding
inline bool decodeGlyph(const std::string &s, glyph &g) {
  std::stringstream ss(s);
  if (!(ss >> g.id))
    return false;
  ss.ignore(1, ';');
  if (!(ss >> g.atlasTop))
    return false;
  ss.ignore(1, ';');
  if (!(ss >> g.atlasLeft))
    return false;
  ss.ignore(1, ';');
  if (!(ss >> g.atlasRight))
    return false;
  ss.ignore(1, ';');
  if (!(ss >> g.atlasBottom))
    return false;
  ss.ignore(1, ';');
  if (!(ss >> g.left))
    return false;
  ss.ignore(1, ';');
  if (!(ss >> g.bottom))
    return false;
  ss.ignore(1, ';');
  if (!(ss >> g.right))
    return false;
  ss.ignore(1, ';');
  if (!(ss >> g.top))
    return false;
  return true;
}

inline std::string encodeGlyphs(const std::vector<glyph> &glyphs) {
  std::string encodedStr = "";
  for (auto const &g : glyphs) {
    encodedStr += encodeGlyph(g);
  }
  return encodedStr;
}

inline bool decodeGlyphs(const std::string &str, std::vector<glyph> &glyphs) {
  std::stringstream ss(str);
  std::string encodedGlyph;
  while (std::getline(ss, encodedGlyph)) {
    glyph g;
    auto success = decodeGlyph(encodedGlyph, g);
    if (!success)
      return success;
    glyphs.push_back(g);
  }
  return true;
}

} // namespace Core
