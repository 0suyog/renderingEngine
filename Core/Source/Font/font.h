#pragma once
#include "Font/fontLib.h"
#include "utf8Str.h"
#include "freetype/freetype.h"
#include <exception>
#include <filesystem>
#include <freetype2/freetype/freetype.h>
#include <optional>
#include <type_traits>
#include <vector>
namespace Core {
class FontCollection {
  /**
   * @brief loads provided fonts from disk if one path is invalid then none of
   * the fonts are loaded
   *
   * @tparam Args anything that can construct std::filesystem::path ie string,
   * *char
   * @param first atleast one path is required
   * @param args all other path to be loaded
   * @return is a constructor, creates FontCollection object
   */
  template <typename... Args>
    requires(std::is_constructible_v<std::filesystem::path, Args> && ...)
  FontCollection(std::filesystem::path first, Args... args) {
    try {

      FontLib::get().loadFont(first);
      (FontLib::get().loadFont(args) && ...);
    } catch (std::exception e) {
      m_fontFaces.clear();
      throw e;
    }
  }

  std::optional<FT_Face> selectFont(rune r);

private:
  std::vector<FT_Face> m_fontFaces;
};

} // namespace Core
