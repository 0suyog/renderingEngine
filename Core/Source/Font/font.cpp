#include "Font/font.h"
#include "Renderer/mesh.h"
#include "Renderer/text.h"
#include "core/BitmapRef.hpp"
#include "freetype/freetype.h"
#include "freetype/ftsystem.h"
#include "hb-ft.h"
#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/ImmediateAtlasGenerator.h"
#include "msdf-atlas-gen/TightAtlasPacker.h"
#include "msdf-atlas-gen/glyph-generators.h"
#include "transform.h"
#include <cstdlib>
#include <iostream>
namespace Core {

static FT_Library ftLibrary = FT_Library();

Renderer::RenderableText shapedGlyphsToRenderableText(
    Font font, const std::vector<ShapedGlyph> &shapedGlyphs,
    const glm::vec3 &normal, const Transform &transform,
    const glm::vec4 &foregroundColor, const glm::vec4 &backgroundColor) {
  msdfgen::BitmapConstSection<unsigned char, 3> atlas = font.getAtlas();
  const std::vector<msdf_atlas::GlyphGeometry> &glyphs = font.getGlyphs();
  std::vector<Renderer::MeshGeometry> meshGeometries;
  std::vector<Renderer::Vertex> vertices;
  std::vector<unsigned int> indices;
  glm::vec3 pen = glm::vec3(0.0f);
  for (const auto &sg : shapedGlyphs) {
    auto glyph =
        std::ranges::find_if(glyphs.begin(), glyphs.end(),
                             [sg](const msdf_atlas::GlyphGeometry &glyph) {
                               return glyph.getGlyphIndex().getIndex() == sg.id;
                             });
    if (glyph == glyphs.end()) {
      std::cerr << "Glyph not found for codePoint" << sg.id << "\n";
    }
    int x, y, w, h;
    glyph->getAdvance();
    glyph->getBoxRect(x, y, w, h);
    // std::cerr << "x: " << x << " y:" << y << " w: " << w << " h:" << h
    //           << "atlas width: " << atlas.width
    //           << " atlas height: " << atlas.height << "\n";

    Renderer::Vertex topLeft = Renderer::Vertex{
        .position = pen,
        .normal = normal,
        .texCoord = {float(x) / atlas.width, float(y) / atlas.height}};
    std::cerr << "topLeft: " << "x: " << topLeft.position.x
              << " y: " << topLeft.position.y << " z: " << topLeft.position.z
              << "\n";
    Renderer::Vertex topRight = Renderer::Vertex{
        .position = pen + glm::vec3({w, 0, 0}),
        .normal = normal,
        .texCoord = {float(x + w) / atlas.width, float(y) / atlas.height}};
    std::cerr << "topRight: " << "x: " << topRight.position.x
              << " y: " << topRight.position.y << " z: " << topRight.position.z
              << "\n";
    Renderer::Vertex bottomRight = Renderer::Vertex{
        .position = pen + glm::vec3({w, h, 0}),
        .normal = normal,
        .texCoord = {float(x + w) / atlas.width, float(y + h) / atlas.height}};
    std::cerr << "bottomRight: " << "x: " << bottomRight.position.x
              << " y: " << bottomRight.position.y
              << " z: " << bottomRight.position.z << "\n";

    Renderer::Vertex bottomLeft = Renderer::Vertex{
        .position = pen + glm::vec3({0, h, 0}),
        .normal = normal,
        .texCoord = {float(x + w) / atlas.width, float(y + h) / atlas.height}};
    std::cerr << "bottomLeft: " << "x: " << bottomLeft.position.x
              << " y: " << bottomLeft.position.y
              << " z: " << bottomLeft.position.z << "\n";
    pen = pen + glm::vec3({w, 0, 0});
    vertices.insert(vertices.end(),
                    {topLeft, topRight, bottomRight, bottomLeft});
    unsigned int size = indices.size();
    indices.insert(indices.end(), {0 + size, 1 + size, 2 + size, 0 + size,
                                   2 + size, 3 + size});
  }
  auto meshGeometry = Renderer::MeshGeometry(vertices, indices);
  meshGeometries.push_back(meshGeometry);
  return Renderer::RenderableText{
      .meshGeometryes = meshGeometries,
      .transform = transform,
      .foregroundColor = foregroundColor,
      .backgroundColor = backgroundColor,
      // set atlas size
      // TODO: make this somewhat good rn its just hardcoded
      .atlas = font.getAtlasTexture(150, 150)};
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

  generateGlyphs(width, height);
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
    generateAtlas();
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

Font::Font(const FT_Face &ftFace, const std::filesystem::path &path, int width,
           int height)
    : m_ftFace(ftFace), path(path) {
  m_msdfFont = msdfgen::adoptFreetypeFont(m_ftFace);
  m_hbFont = hb_ft_font_create_referenced(m_ftFace);
  getAtlasTexture(width, height);
}

Font::~Font() {
  if (m_hbFont) {
    hb_free(m_hbFont);
  }
  if (m_ftFace) {
    FT_Done_Face(m_ftFace);
  }
}

std::vector<ShapedGlyph> Font::shapeText(const std::string &text) const {
  hb_buffer_t *buffer = hb_buffer_create();
  hb_buffer_add_utf8(buffer, text.c_str(), static_cast<int>(text.size()), 0,
                     static_cast<int>(text.size()));
  hb_buffer_guess_segment_properties(buffer);
  assert(m_hbFont);
  hb_shape(m_hbFont, buffer, nullptr, 0);
  unsigned int count = 0;
  auto *infos = hb_buffer_get_glyph_infos(buffer, &count);
  std::cerr << "letter: " << text[0] << "codepoint: " << infos[0].codepoint
            << "\n";
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

// TODO: rn i am packing atlas twice, one when generating glyph, and another
// when generating atlas as glyphs need to be packed, and since i am caching
// atlas as image and don't have information as to where each glyph is placed in
// atlas i need to pack them again (i am not sure if its consistent ie if
// packing same char set will result in same atlas same time will know if
// something breaks ig), instead of doing that cache both the atlas and
// the glyphs, and restore them with atlas
void Font::generateGlyphs(int width, int height) {
  std::vector<msdf_atlas::GlyphGeometry> glyphs;
  msdf_atlas::FontGeometry geometry(&glyphs);
  assert(m_ftFace);
  auto error = FT_Set_Char_Size(m_ftFace, width, height, 300, 300);
  if (error) {
    std::cerr << "failed setting font size\n";
  }

  assert(m_msdfFont);

  bool loaded =
      geometry.loadCharset(m_msdfFont, 1.0, msdf_atlas::Charset::ASCII);
  std::cerr << "\n";
  for (auto &glyph : glyphs) {
    glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, 3.0, 0);
    // if (glyph.getGlyphIndex().getIndex() == 10) {
    //   std::cerr << "found?";
    // }
    // std::cerr << glyph.getCodepoint() << "\n";
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
  // exit(-1);
  m_glyphs = glyphs;
}

void Font::generateAtlas() {

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
  auto error = packer.pack(m_glyphs.data(), m_glyphs.size());
  if (error) {
    std::cerr << "error when packing atlas\n";
  }
  int width, height;
  packer.getDimensions(width, height);

  msdf_atlas::ImmediateAtlasGenerator<
      float, 3, msdf_atlas::msdfGenerator,
      msdf_atlas::BitmapAtlasStorage<unsigned char, 3>>
      generator(width, height);
  generator.generate(m_glyphs.data(), m_glyphs.size());
  m_atlas = generator.atlasStorage();
  msdfgen::BitmapRef<unsigned char, 3> m = m_atlas;
}

/**
 * @brief Loads provided font from path with provided width and height, leave
 * height to 0 to make it same as width
 * @param path font path
 * @param width font width def:100
 * @param height font height def:100
 * @return
 */
std::shared_ptr<Font> loadFont(const std::filesystem::path &path, int width,
                               int height) {
  if (!ftLibrary) {
    initFontLoader();
  }
  if (fontMap.contains(path.string())) {
    return fontMap[path.string()];
  }

  FT_Face ftFace;
  auto err = FT_New_Face(ftLibrary, path.c_str(), 0, &ftFace);
  if (err) {
    if (err) {
      std::cerr << "Failed creating FT face: " << FT_Error_String(err) << '\n';
    }
  }
  fontMap[path.string()] =
      std::shared_ptr<Font>(new Font(ftFace, path, width, height));
  return fontMap[path.string()];
}

} // namespace Core
