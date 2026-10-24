#pragma once
#include "Renderer/mesh.h"
#include "Renderer/texture.h"
#include "core/BitmapRef.hpp"
#include "glm/ext/vector_float3.hpp"
#include "hb-ft.h"
#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include "msdf-atlas-gen/ImmediateAtlasGenerator.h"
#include "msdf-atlas-gen/TightAtlasPacker.h"
#include "msdf-atlas-gen/glyph-generators.h"
#include <cstdint>
#include <ft2build.h>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include FT_FREETYPE_H

namespace Renderer {

struct RenderableText {
  std::vector<Mesh> meshes;
  Material mat;
};

struct ShapedGlyph {
  uint32_t id;
  float xAdvance, yAdvance;
  float xOffset, yOffset;
};

inline RenderableText shapedGlyphsToRenderableText(
    const msdfgen::BitmapSection<unsigned char, 3> &atlas,
    std::vector<msdf_atlas::GlyphGeometry> glyphs,
    const std::vector<ShapedGlyph> &shapedGlyphs, const glm::vec3 &normal,
    const Material &mat) {
  std::vector<Mesh> meshes;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  glm::vec3 pen = glm::vec3(0.0f);
  for (const auto &sg : shapedGlyphs) {
    auto glyph =
        std::ranges::find_if(glyphs.begin(), glyphs.end(),
                             [sg](const msdf_atlas::GlyphGeometry &glyph) {
                               return glyph.getCodepoint() == sg.id;
                             });
    if (glyph == glyphs.end()) {
      std::cerr << "Glyph not found for codePoint" << sg.id << "\n";
    }
    int x, y, w, h;
    glyph->getBoxRect(x, y, w, h);

    Vertex topLeft =
        Vertex{.position = pen,
               .normal = normal,
               .texCoord = {float(x) / atlas.width, float(y) / atlas.height}};
    Vertex topRight = Vertex{
        .position = pen + glm::vec3({w, 0, 0}),
        .normal = normal,
        .texCoord = {float(x + w) / atlas.width, float(y) / atlas.height}};
    Vertex bottomRight = Vertex{
        .position = pen + glm::vec3({w, h, 0}),
        .normal = normal,
        .texCoord = {float(x + w) / atlas.width, float(y + h) / atlas.height}};

    Vertex bottomLeft = Vertex{
        .position = pen + glm::vec3({0, h, 0}),
        .normal = normal,
        .texCoord = {float(x + w) / atlas.width, float(y + h) / atlas.height}};
    vertices.insert(vertices.end(),
                    {topLeft, topRight, bottomRight, bottomLeft});
    unsigned int size = indices.size();
    indices.insert(indices.end(), {0 + size, 1 + size, 2 + size, 0 + size,
                                   2 + size, 3 + size});
  }
  Mesh m{vertices, indices, mat};
  meshes.push_back(m);
  return RenderableText{.meshes = meshes, .mat = mat};
}

inline FT_Library ftLibrary = FT_Library();

inline void initFontLoader() {
  std::cerr << "fails here\n";
  FT_Init_FreeType(&ftLibrary);
  std::cerr << "this doesnt run";
}

inline void freeFontLibrary() {
  std::cerr << "free font lib\n";
  FT_Free_Func(ftLibrary);
  std::cerr << "free font lib fin\n";
}

inline std::unordered_map<std::string, std::shared_ptr<Font>> fontMap;

class Font {
public:
  Font(const FT_Face &ftFace) : m_ftFace(ftFace) {
    // init font
    m_hbFont = hb_ft_font_create_referenced(ftFace);
    // init font done
  }

  std::shared_ptr<Texture> getAtlasTexture(unsigned int width,
                                           unsigned int height) {
    auto key = std::pair<unsigned int, unsigned int>(width, height);
    if (m_texturesMap.contains(key)) {
      return m_texturesMap[key];
    }
    generateAtlas(width, height);
    m_texturesMap[key] = AtlasTexture(*this);
    return m_texturesMap[key];
  }

  ~Font() {
    hb_free(m_hbFont);
    FT_Free_Func(ftFace);
  }

  const msdf_atlas::BitmapAtlasStorage<unsigned char, 3> getAtlas() const {
    return m_atlas;
  }

  std::vector<ShapedGlyph> shapeText(const std::string &text) {
    hb_buffer_t *buffer = hb_buffer_create();
    hb_buffer_add_utf8(buffer, text.c_str(), static_cast<int>(text.size()), 0,
                       static_cast<int>(text.size()));
    hb_buffer_guess_segment_properties(buffer);
    hb_shape(m_hbFont, buffer, nullptr, 0);
    unsigned int count = 0;
    auto *infos = hb_buffer_get_glyph_infos(buffer, &count);
    auto *positions = hb_buffer_get_glyph_positions(buffer, &count);
    std::vector<ShapedGlyph> results;
    results.reserve(count);
    for (unsigned int i = 0; i < count; i++) {
      auto thingy = ShapedGlyph{.id = infos[i].codepoint,
                                .xAdvance = positions[i].x_advance / 64.0f,
                                .yAdvance = positions[i].y_advance / 64.0f,
                                .xOffset = positions[i].x_offset / 64.0f,
                                .yOffset = positions[i].y_offset / 64.0f};
      results.push_back(thingy);
    }
    hb_buffer_destroy(buffer);
    return results;
  }

  void generateAtlas(int width, int height) {
    msdfgen::FreetypeHandle *msdft = msdfgen::initializeFreetype();
    msdfgen::FontHandle *msdffont = msdfgen::loadFont(
        msdft, "/usr/share/fonts/IsovekaTerm/IosevkaTermNerdFont-Heavy.ttf");

    std::vector<msdf_atlas::GlyphGeometry> glyphs;
    msdf_atlas::FontGeometry geometry(&glyphs);

    geometry.loadCharset(msdffont, 1.0, msdf_atlas::Charset::ASCII);

    for (auto &glyph : glyphs) {
      glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, 3.0, 0);
    }

    msdf_atlas::TightAtlasPacker packer;
    packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
    packer.setMinimumScale(240.0);
    packer.setPixelRange(2.0);
    packer.setMiterLimit(1.0);
    packer.pack(glyphs.data(), glyphs.size());
    packer.getDimensions(width, height);

    msdf_atlas::ImmediateAtlasGenerator<
        float, 3, msdf_atlas::msdfGenerator,
        msdf_atlas::BitmapAtlasStorage<unsigned char, 3>>
        generator(width, height);
    generator.generate(glyphs.data(), glyphs.size());

    m_atlas = generator.atlasStorage();
    // msdfgen::BitmapConstRef<unsigned char, 3> m = atlas;

    // Core::WritePng("atlas.png", m.width, m.height, 3, m.pixels);
  }

private:
  msdf_atlas::BitmapAtlasStorage<unsigned char, 3> m_atlas;
  std::vector<msdf_atlas::GlyphGeometry> m_glyphs;
  FT_Face m_ftFace;
  hb_font_t *m_hbFont;
  std::map<std::pair<unsigned int, unsigned int>, std::shared_ptr<Texture>>
      m_texturesMap;
};

/**
 * @brief Loads provided font with provided width and height, leave height as 0
 * to make it same as width
 *
 * @param path font path
 * @return
 */
inline std::shared_ptr<Font> loadFont(const std::filesystem::path &path) {
  if (fontMap.contains(path.string())) {
    return fontMap[path.string()];
  }

  FT_Face ftFace;
  FT_New_Face(ftLibrary, path.c_str(), 0, &ftFace);
  fontMap[path.string()] = std::make_shared<Font>(ftFace);
  return fontMap[path.string()];
}

} // namespace Renderer
