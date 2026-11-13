#pragma once
#include <cstdint>
namespace Core {

struct glyph {
  // glyph index in font
  uint32_t id;
  // position in atlas
  double atlasLeft, atlasTop, atlasRight, atlasBottom;
  // where the glyph should be placed on baseline
  double left, bottom, right, top;
};

struct ShapedGlyph {
  // glyph index in font
  uint32_t id;
  // how much to move the pen after this glyph
  float xAdvance, yAdvance;
  // how much to move the penposition before rendering this glyph
  float xOffset, yOffset;
  // is newline
  bool isNewLine = false;
};
} // namespace Core
