#include "Font/fontLib.h"
#include "assimp/Exceptional.h"
#include "freetype/freetype.h"
#include <cassert>
#include <iostream>
#include <stdexcept>
namespace Core {

static FontLib *library;

void FontLib::initializeFreeType() {
  m_library = std::make_unique<FT_Library>();
  static auto error = FT_Init_FreeType(m_library.get());

  if (error) {
    throw std::runtime_error("Error initializing freetype library");
  }
}

const FT_Library &FontLib::getLibrary() const { return *m_library; }

const FontLib &FontLib::get() {
  if (!library) {
    throw runtime_error("freetype library isn't initialized");
  }
  return *library;
}

FT_Face FontLib::loadFont(std::filesystem::path path) const {
  if (!m_library)
    throw std::runtime_error("freetype library isn't initialized");

  FT_Face face;

  auto error = FT_New_Face(*m_library, path.c_str(), 0, &face);
  if (!error)
    return face;

  throw std::runtime_error("Failed loading font");
}

} // namespace Core
  // namespace Core
