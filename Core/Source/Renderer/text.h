#pragma once
#include "Renderer/mesh.h"
#include "Renderer/regularPolygon.h"
#include "core/edge-coloring.h"
#include "ext/import-font.h"
#include "file.h"
#include "glm/ext/vector_float3.hpp"
#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/ImmediateAtlasGenerator.h"
#include "msdf-atlas-gen/TightAtlasPacker.h"
#include "msdf-atlas-gen/types.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include <hb.h>
#include <hb-ft.h>

#include <msdf-atlas-gen/msdf-atlas-gen.h>

#include <cstdint>
#include <string>
#include <vector>
namespace Renderer {

struct ShapedGlyph {
  uint32_t id;
  float xAdvance, yAdvance;
  float xOffset, yOffset;
};

inline FT_Library ftLibrary;
inline FT_Face ftFace;
inline hb_font_t *hbFont;

class Text {
public:
  Text() {
    // init font
    FT_Init_FreeType(&ftLibrary);
    FT_New_Face(ftLibrary,
                "/usr/share/fonts/IsovekaTerm/IosevkaTermNerdFont-Heavy.ttf", 0,
                &ftFace);
    FT_Set_Pixel_Sizes(ftFace, 0, 40);
    hbFont = hb_ft_font_create_referenced(ftFace);
    // init font done
  }

  std::vector<ShapedGlyph> shapeText(const std::string &text) {
    hb_buffer_t *buffer = hb_buffer_create();
    hb_buffer_add_utf8(buffer, text.c_str(), static_cast<int>(text.size()), 0,
                       static_cast<int>(text.size()));
    hb_buffer_guess_segment_properties(buffer);
    hb_shape(hbFont, buffer, nullptr, 0);
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

  void renderText(const std::string &s) {
    auto shapedText = shapeText(s);
    float penX = 100.0f;
    float penY = 100.0f;
    for (const auto &st : shapedText) {
      auto w = fabs(st.xOffset - st.xAdvance);
      auto h = fabs(st.yOffset - st.yAdvance);
      auto s = Renderer::RegularPolygon(fmax(w, h), 4);
    }
  }

private:
  void generateAtlas() {
    msdfgen::FreetypeHandle *msdft = msdfgen::initializeFreetype();
    msdfgen::FontHandle *msdffont = msdfgen::loadFont(
        msdft, "/usr/share/fonts/IsovekaTerm/IosevkaTermNerdFont-Heavy.ttf");

    std::vector<msdf_atlas::GlyphGeometry> glyphs;
    msdf_atlas::FontGeometry geometry(&glyphs);

    for (auto &glyph : glyphs) {
      glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, 3.0, 0);
    }

    msdf_atlas::TightAtlasPacker packer;
    packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
    packer.setPixelRange(4.0f);
    packer.setMinimumScale(48.0f);
    packer.pack(glyphs.data(), glyphs.size());
    int width, height;
    packer.getDimensions(width, height);

    msdf_atlas::ImmediateAtlasGenerator<
        float, 3, msdf_atlas::msdfGenerator,
        msdf_atlas::BitmapAtlasStorage<unsigned char, 3>>
        generator(width, height);
    generator.generate(glyphs.data(), glyphs.size());
    msdf_atlas::BitmapAtlasStorage<unsigned char, 3> bitmap =
        generator.atlasStorage();
    msdfgen::BitmapRef<unsigned char, 3> m = bitmap;
    Core::WritePng("atlas.png", m.width, m.height, 3, m.pixels);
  }

  std::vector<Mesh> m_meshes;
  Material m_mat;
};
} // namespace Renderer
