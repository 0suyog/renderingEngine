#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include "ext/import-font.h"
#include "Renderer/texture.h"
#include "glm/ext/vector_float3.hpp"
#include "hb.h"
#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include "transform.h"
#include "utils.h"
#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace Renderer {
struct RenderableText;
}

namespace Core {

struct ShapedGlyph {
  uint32_t id;
  float xAdvance, yAdvance;
  float xOffset, yOffset;
};

void initFontLoader();

void freeFontLibrary();

class Font {
public:
  std::shared_ptr<Renderer::Texture> getAtlasTexture(unsigned int width,
                                                     unsigned int height);

  ~Font();

  const msdf_atlas::BitmapAtlasStorage<unsigned char, 3> getAtlas() const {
    return m_atlas;
  }

  const std::vector<msdf_atlas::GlyphGeometry> &getGlyphs() const {
    return m_glyphs;
  }

  std::vector<ShapedGlyph> shapeText(const std::string &text) const;

  void generateGlyphs(int width, int height);

  void generateAtlas();

  friend std::shared_ptr<Font> loadFont(const std::filesystem::path &path,
                                        int width, int height);

private:
  Font(const FT_Face &ftFace, const std::filesystem::path &path, int width,
       int height);

  msdf_atlas::BitmapAtlasStorage<unsigned char, 3> m_atlas;
  std::vector<msdf_atlas::GlyphGeometry> m_glyphs;
  FT_Face m_ftFace = nullptr;
  hb_font_t *m_hbFont = nullptr;
  msdfgen::FontHandle *m_msdfFont = nullptr;
  std::unordered_map<Core::UIntHash, std::shared_ptr<Renderer::Texture>>
      m_texturesMap;
  std::filesystem::path path;
};

Renderer::RenderableText shapedGlyphsToRenderableText(
    std::shared_ptr<Font> font, const std::vector<ShapedGlyph> &shapedGlyphs,
    const glm::vec3 &normal, const Transform &transform,
    const glm::vec4 &foregroundColor = {0, 0, 0, 1},
    const glm::vec4 &backgroundColor = {1, 1, 1, 1});

inline std::unordered_map<std::string, std::shared_ptr<Font>> fontMap;

std::string fontAtlasCacheName(std::filesystem::path path,
                               uint64_t sizePairHash);

inline std::filesystem::path cacheDir = "./cache/";

bool cacheFontAtlas(const Font &font, const std::string &cacheName);

inline std::shared_ptr<Renderer::Texture>
getCachedFontAtlas(const std::string &cacheName);

/**
 * @brief Loads provided font with provided width and height, leave height as 0
 * to make it same as width
 *
 * @param path font path
 * @return
 */
std::shared_ptr<Font> loadFont(const std::filesystem::path &path,
                               int width = 100, int height = 0);

} // namespace Core
