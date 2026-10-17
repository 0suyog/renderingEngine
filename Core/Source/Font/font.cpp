#include "Font/font.h"
#include <iostream>
#include <optional>
namespace Core {
std::optional<FT_Face> FontCollection::selectFont(rune r) {
  auto &lib = FontLib::get().getLibrary();

  for (const auto &face : m_fontFaces) {
    auto glyph_index = FT_Get_Char_Index(face, r);
    if (glyph_index == r) {
      std::cerr << "[FontCollection] char map isn't selected";
    }
    if (glyph_index) {
      return std::make_optional(face);
    }
  }

  return nullptr;
}

} // namespace Core
