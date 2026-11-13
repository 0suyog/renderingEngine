#pragma once
#include "Font/glyph.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "ext/import-font.h"
#include "Renderer/texture.h"
#include "glm/ext/vector_float3.hpp"
#include "hb.h"
#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include "transform.h"
#include "coreUtils.h"
#include <cstdint>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace Renderer {
struct RenderableText;
}

namespace Core {

void initFontLoader();

void freeFontLibrary();

class Font {
public:
  std::shared_ptr<Renderer::Texture> getAtlasTexture();

  ~Font();

  const msdf_atlas::BitmapAtlasStorage<unsigned char, 3> getAtlas() const {
    return m_atlas;
  }

  const std::vector<glyph> &getGlyphs() const { return m_glyphs; }

  hb_font_t *getHbFont() const { return m_hbFont; }

  const FT_Face &getFtFace() const { return m_ftFace; }

  std::vector<ShapedGlyph> shapeText(const std::string &text) const;

  void setSize(int width, int height = 0);

  friend std::shared_ptr<Font> loadFont(const std::filesystem::path &path,
                                        int width, int height);
  int getWidth() { return m_width; }
  int getHeight() { return m_height; }

private:
  std::vector<msdf_atlas::GlyphGeometry> generateGlyphGeometries() const;
  void
  generateGlyphs(const std::vector<msdf_atlas::GlyphGeometry> &glyphGeometries);
  void generateAtlas(std::vector<msdf_atlas::GlyphGeometry> &glyphGeometries);

private:
  Font(const FT_Face &ftFace, const std::filesystem::path &path, int width,
       int height);
  msdf_atlas::BitmapAtlasStorage<unsigned char, 3> m_atlas;
  std::vector<glyph> m_glyphs;
  std::vector<msdf_atlas::GlyphGeometry> m_glyphGeometries;
  FT_Face m_ftFace = nullptr;
  hb_font_t *m_hbFont = nullptr;
  msdfgen::FontHandle *m_msdfFont = nullptr;
  std::shared_ptr<Renderer::Texture> m_atlasTexture;
  std::unordered_map<Core::UIntHash, std::shared_ptr<Renderer::Texture>>
      m_texturesMap;
  std::filesystem::path path;
  int m_width, m_height;
};

Renderer::RenderableText shapedGlyphsToRenderableText(
    std::shared_ptr<Font> font, const std::vector<ShapedGlyph> &shapedGlyphs,
    const glm::vec3 &normal, const Transform &transform,
    const glm::vec4 &foregroundColor, const glm::vec4 &backgroundColor);

inline std::unordered_map<std::string, std::shared_ptr<Font>> fontMap;

std::string fontAtlasCacheName(std::filesystem::path path);

inline std::filesystem::path cacheDir = "./cache/";

bool cacheFontAtlas(const Font &font, const std::string &cacheName);

inline std::shared_ptr<Renderer::Texture>
getCachedFontAtlas(const std::string &cacheName);

bool cacheGlyphs(const Font &font, const std::string &cacheName);

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
