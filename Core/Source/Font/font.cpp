#include "Font/font.h"
#include "Renderer/text.h"
#include "core/BitmapRef.hpp"
#include "hb-ft.h"
#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/ImmediateAtlasGenerator.h"
#include "msdf-atlas-gen/TightAtlasPacker.h"
#include "msdf-atlas-gen/glyph-generators.h"
#include <iostream>
namespace Core {
Renderer::RenderableText
shapedGlyphsToRenderableText(Font font,
                             const std::vector<ShapedGlyph> &shapedGlyphs,
                             const glm::vec3 &normal) {
  msdfgen::BitmapConstSection<unsigned char, 3> atlas = font.getAtlas();
  std::vector<msdf_atlas::GlyphGeometry> glyphs = font.getGlyphs();
  std::vector<Renderer::Mesh> meshes;
  std::vector<Renderer::Vertex> vertices;
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

    Renderer::Vertex topLeft = Renderer::Vertex{
        .position = pen,
        .normal = normal,
        .texCoord = {float(x) / atlas.width, float(y) / atlas.height}};
    Renderer::Vertex topRight = Renderer::Vertex{
        .position = pen + glm::vec3({w, 0, 0}),
        .normal = normal,
        .texCoord = {float(x + w) / atlas.width, float(y) / atlas.height}};
    Renderer::Vertex bottomRight = Renderer::Vertex{
        .position = pen + glm::vec3({w, h, 0}),
        .normal = normal,
        .texCoord = {float(x + w) / atlas.width, float(y + h) / atlas.height}};

    Renderer::Vertex bottomLeft = Renderer::Vertex{
        .position = pen + glm::vec3({0, h, 0}),
        .normal = normal,
        .texCoord = {float(x + w) / atlas.width, float(y + h) / atlas.height}};
    vertices.insert(vertices.end(),
                    {topLeft, topRight, bottomRight, bottomLeft});
    unsigned int size = indices.size();
    indices.insert(indices.end(), {0 + size, 1 + size, 2 + size, 0 + size,
                                   2 + size, 3 + size});
  }
  Renderer::Mesh m{vertices, indices};
  meshes.push_back(m);
  return Renderer::RenderableText{.meshes = meshes};
}

void initFontLoader() { FT_Init_FreeType(&ftLibrary); }

void freeFontLibrary() { FT_Free_Func(ftLibrary); }

std::string fontAtlasCacheName(std::filesystem::path path,
                               uint64_t sizePairHash) {
  auto name = path.filename();
  path.append(std::to_string(sizePairHash));
  return name.string() + std::to_string(std::filesystem::hash_value(path));
}

bool cacheFontAtlas(const Font &font, const std::string &cacheName) {
  auto atlas = font.getAtlas();
  msdfgen::BitmapConstRef<unsigned char, 3> bmp = atlas;
  auto cachePath = cacheDir;
  cachePath.append(cacheName);
  return Core::WritePng(cachePath, bmp.width, bmp.height, 3, bmp.pixels);
}

std::optional<msdf_atlas::BitmapAtlasStorage<unsigned char, 3>>
generateAtlasFromCache(const std::string &cacheName) {
  auto cachePath = cacheDir;
  cachePath.append(cacheName);
  auto im = Core::ReadImage(cachePath);
  if (!im)
    return std::optional<msdf_atlas::BitmapAtlasStorage<unsigned char, 3>>();

  msdfgen::BitmapConstSection<unsigned char, 3> sec;
  sec.pixels = im->data;
  sec.width = im->width;
  sec.height = im->height;
  sec.rowStride = im->channels * im->width;
  msdf_atlas::BitmapAtlasStorage<unsigned char, 3> atlas =
      msdf_atlas::BitmapAtlasStorage<unsigned char, 3>(sec.width, sec.height);
  atlas.put(0, 0, sec);
  return atlas;
}

std::shared_ptr<Renderer::Texture> Font::getAtlasTexture(unsigned int width,
                                                         unsigned int height) {
  auto key = Core::hashUIntPair(width, height);
  if (m_texturesMap.contains(key)) {
    return m_texturesMap[key];
  }
  auto cacheName = fontAtlasCacheName(path, key);
  if (auto cachedAtlas = generateAtlasFromCache(cacheName)) {
    m_atlas = cachedAtlas.value();
    msdfgen::BitmapConstRef<unsigned char, 3> bmp = m_atlas;
    m_texturesMap[key] =
        Renderer::TextureFromMemory(bmp.pixels, bmp.width, bmp.height, 3, 3);
  } else {
    generateAtlas(width, height);
    msdfgen::BitmapConstRef<unsigned char, 3> bmp = m_atlas;
    m_texturesMap[key] =
        Renderer::TextureFromMemory(bmp.pixels, bmp.width, bmp.height, 3, 3);
    auto success = cacheFontAtlas(*this, cacheName);
    if (!success) {
      std::cerr << "failed caching atlas";
    }
  }
  return m_texturesMap[key];
}

Font::Font(const FT_Face &ftFace, const std::filesystem::path &path)
    : m_ftFace(ftFace), path(path) {
  m_hbFont = hb_ft_font_create_referenced(ftFace);
}

Font::~Font() {
  hb_free(m_hbFont);
  FT_Free_Func(ftFace);
}

std::vector<ShapedGlyph> Font::shapeText(const std::string &text) const {
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

void Font::generateAtlas(int width, int height) {
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
  // this sets the minimum "space a glyph will take" so big number better if
  // viewing letters from close
  packer.setMinimumScale(240.0);
  // how much information needs to be retained outside of an edge basically to
  // know how far i am from an edge, helpful for glowing texts and stuff
  packer.setPixelRange(10.0);
  // couldnt figure out this tho
  // TODO: figure oute what this does (I couldn't see difference between miter
  // of 0.1 and 40, other than 0.1 is a teeny bit blurrier than 40)
  packer.setMiterLimit(0.1);
  packer.pack(glyphs.data(), glyphs.size());
  packer.getDimensions(width, height);

  msdf_atlas::ImmediateAtlasGenerator<
      float, 3, msdf_atlas::msdfGenerator,
      msdf_atlas::BitmapAtlasStorage<unsigned char, 3>>
      generator(width, height);
  generator.generate(glyphs.data(), glyphs.size());

  m_atlas = generator.atlasStorage();
  msdfgen::BitmapRef<unsigned char, 3> m = m_atlas;
}

/**
 * @brief Loads provided font with provided width and height, leave height as 0
 * to make it same as width
 *
 * @param path font path
 * @return
 */
std::shared_ptr<Font> loadFont(const std::filesystem::path &path) {
  if (fontMap.contains(path.string())) {
    return fontMap[path.string()];
  }

  FT_Face ftFace;
  FT_New_Face(ftLibrary, path.c_str(), 0, &ftFace);
  fontMap[path.string()] = std::shared_ptr<Font>(new Font(ftFace, path));
  return fontMap[path.string()];
}

} // namespace Core
