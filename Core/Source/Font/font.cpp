#include "Font/font.h"
#include "Font/fontUtils.h"
#include "Font/glyph.h"
#include "Renderer/mesh.h"
#include "Renderer/text.h"
#include "core/BitmapRef.hpp"
#include "file.h"
#include "freetype/freetype.h"
#include "freetype/ftsystem.h"
#include "hb-ft.h"
#include "msdf-atlas-gen/BitmapAtlasStorage.h"
#include "msdf-atlas-gen/FontGeometry.h"
#include "msdf-atlas-gen/GlyphGeometry.h"
#include "msdf-atlas-gen/ImmediateAtlasGenerator.h"
#include "msdf-atlas-gen/TightAtlasPacker.h"
#include "msdf-atlas-gen/glyph-generators.h"
#include "transform.h"
#include <iostream>
#include <vector>
namespace Core {

static FT_Library ftLibrary = FT_Library();

void initFontLoader() { FT_Init_FreeType(&ftLibrary); }

void freeFontLibrary() { FT_Free_Func(ftLibrary); }

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

Renderer::RenderableText shapedGlyphsToRenderableText(
    std::shared_ptr<Font> font,
    const std::vector<std::vector<ShapedGlyph>> &shapedLines,
    const glm::vec3 &normal, const Transform &transform,
    const glm::vec4 &foregroundColor, const glm::vec4 &backgroundColor) {

  msdfgen::BitmapConstSection<unsigned char, 3> atlas = font->getAtlas();
  auto &glyphs = font->getGlyphs();

  std::vector<Renderer::MeshGeometry> meshGeometries;

  glm::vec3 baseLine = glm::vec3(0.0f);

  const double emScale = 1.0 / font->getHeight();
  float lineHeight = font->getFtFace()->size->metrics.height / 64.0f * emScale;

  std::cerr << "line height: " << lineHeight << "\n";

  for (const auto &sl : shapedLines) {
    glm::vec3 pen = baseLine;
    std::vector<Renderer::Vertex> vertices;
    std::vector<unsigned int> indices;
    for (const auto &sg : sl) {
      auto glyph = std::ranges::find_if(
          glyphs.begin(), glyphs.end(),
          [sg](const struct glyph &glyph) { return glyph.id == sg.id; });

      if (glyph == glyphs.end()) {
        char glyphName[128] = {};

        FT_Error error = FT_Get_Glyph_Name(font->getFtFace(), sg.id, glyphName,
                                           sizeof(glyphName));

        auto gl = FT_Get_Name_Index(font->getFtFace(), glyphName);

        std::cerr << "Glyph not found: " << gl << " | Glyph index: " << sg.id
                  << " | Error: " << error << '\n';
        continue;
      }

      // flipping the v coords cuz textures is vertically flipped so need to
      // unflip it
      int atlasLeft = glyph->atlasLeft, atlasBottom = glyph->atlasTop,
          atlasRight = glyph->atlasRight, atlasTop = glyph->atlasBottom;
      double top = glyph->top, bottom = glyph->bottom, left = glyph->left,
             right = glyph->right;

      // DK: I don't really understand the font scale will have to look it up

      auto xadvance = sg.xAdvance * emScale;
      auto yadvance = sg.yAdvance * emScale;
      auto xoffset = sg.xOffset * emScale;
      auto yoffset = sg.yOffset * emScale;

      auto penWOffsetForGlyph = pen + glm::vec3(xoffset, yoffset, 0);

      int xScale, yScale;

      hb_font_get_scale(font->getHbFont(), &xScale, &yScale);
      Renderer::Vertex topLeft = Renderer::Vertex{
          .position = penWOffsetForGlyph + glm::vec3(left, top, 0),
          .normal = normal,
          .texCoord = {float(atlasLeft) / atlas.width,
                       float(atlasTop) / atlas.height}};

      Renderer::Vertex topRight = Renderer::Vertex{
          .position = penWOffsetForGlyph + glm::vec3(right, top, 0),
          .normal = normal,
          .texCoord = {float(atlasRight) / atlas.width,
                       float(atlasTop) / atlas.height}};

      Renderer::Vertex bottomRight = Renderer::Vertex{
          .position = penWOffsetForGlyph + glm::vec3({right, bottom, 0}),
          .normal = normal,
          .texCoord = {float(atlasRight) / atlas.width,
                       float(atlasBottom) / atlas.height}};

      Renderer::Vertex bottomLeft = Renderer::Vertex{
          .position = penWOffsetForGlyph + glm::vec3({left, bottom, 0}),
          .normal = normal,
          .texCoord = {float(atlasLeft) / atlas.width,
                       float(atlasBottom) / atlas.height}};

      pen = pen + glm::vec3({xadvance, yadvance, 0});

      unsigned int offset = vertices.size();
      vertices.insert(vertices.end(),
                      {topLeft, topRight, bottomRight, bottomLeft});
      indices.insert(indices.end(), {offset, offset + 1, offset + 2, offset,
                                     offset + 2, offset + 3});

      auto meshGeometry = Renderer::MeshGeometry(vertices, indices);
      meshGeometries.push_back(meshGeometry);
    }
    baseLine.y -= lineHeight;
  }

  return Renderer::RenderableText{.meshGeometryes = meshGeometries,
                                  .transform = transform,
                                  .foregroundColor = foregroundColor,
                                  .backgroundColor = backgroundColor,
                                  .atlas = font->getAtlasTexture()};
}

std::string fontAtlasCacheName(std::filesystem::path path) {
  auto name = path.filename();
  return name.string() + std::to_string(std::filesystem::hash_value(path));
}

bool cacheFontAtlas(const Font &font, const std::string &cacheName) {
  auto atlas = font.getAtlas();
  msdfgen::BitmapConstRef<unsigned char, 3> bmp = atlas;
  auto cachePath = cacheDir;
  cachePath.append(cacheName);
  return Core::WritePng(cachePath, bmp.width, bmp.height, 3, bmp.pixels);
}

bool cacheGlyphs(const Font &font, const std::string &cacheName) {
  auto cachePath = cacheDir;
  cachePath.append(cacheName);
  std::cerr << "writing to file for glyphs now\n";
  return WriteTextFile(cachePath, encodeGlyphs(font.getGlyphs()));
}

static bool generateAtlasFromCache(
    const std::string &cacheName,
    msdf_atlas::BitmapAtlasStorage<unsigned char, 3> &atlas) {
  auto cachePath = cacheDir;
  cachePath.append(cacheName);
  auto im = Core::ReadImage(cachePath);
  if (!im)
    return false;

  msdfgen::BitmapConstSection<unsigned char, 3> sec;
  sec.pixels = im->data;
  sec.width = im->width;
  sec.height = im->height;
  sec.rowStride = im->channels * im->width;
  atlas =
      msdf_atlas::BitmapAtlasStorage<unsigned char, 3>(sec.width, sec.height);
  atlas.put(0, 0, sec);
  return true;
}

static bool generateGlyphsFromCache(const std::string &cacheName,
                                    std::vector<glyph> &glyphs) {
  auto cachePath = cacheDir;
  cachePath.append(cacheName);
  auto text = ReadTextFile(cachePath);
  if (!text)
    return false;
  auto success = decodeGlyphs(text.value(), glyphs);
  return success;
}

void Font::setSize(int width, int height) {
  m_width = width;
  if (m_width < 1)
    m_width = 1;
  m_height = height < 1 ? width : height;
  assert(m_ftFace);
  auto error = FT_Set_Pixel_Sizes(m_ftFace, m_width, m_height);

  if (error) {
    std::cerr << "failed setting font size\n";
  }

  assert(m_hbFont);
  hb_ft_font_changed(m_hbFont);
}

std::shared_ptr<Renderer::Texture> Font::getAtlasTexture() {

  if (m_atlasTexture && m_glyphs.size()) {
    return m_atlasTexture;
  }
  auto atlasCacheName = path.filename().replace_extension() / "atlas.png";
  auto glyphsCacheName = path.filename().replace_extension() / "glyphs";

  // only use cache if both atlas cache and glyphs cache is available otherwise
  // generate new
  if (generateAtlasFromCache(atlasCacheName, m_atlas) &&
      generateGlyphsFromCache(glyphsCacheName, m_glyphs) && m_glyphs.size()) {
    std::cerr << "seems both are available\n";
    msdfgen::BitmapConstRef<unsigned char, 3> bmp = m_atlas;
    m_atlasTexture =
        Renderer::TextureFromMemory(bmp.pixels, bmp.width, bmp.height, 3, 3);
  } else {
    m_glyphs.clear();
    auto glyphGeometries = generateGlyphGeometries();
    std::cerr << "generating atlas\n";
    generateAtlas(glyphGeometries);
    std::cerr << "generating glyphs\n";
    generateGlyphs(glyphGeometries);
    std::cerr << "finished generating glyphs\n";
    msdfgen::BitmapConstRef<unsigned char, 3> bmp = m_atlas;
    m_atlasTexture =
        Renderer::TextureFromMemory(bmp.pixels, bmp.width, bmp.height, 3, 3);
    std::cerr << "caching atlas\n";
    auto success = cacheFontAtlas(*this, atlasCacheName);
    if (!success) {
      std::cerr << "failed caching atlas";
    }
    success = cacheGlyphs(*this, glyphsCacheName);
    if (!success) {
      std::cerr << "failed caching glyphs\n";
    }
  }
  return m_atlasTexture;
}

Font::Font(const FT_Face &ftFace, const std::filesystem::path &path, int width,
           int height)
    : m_ftFace(ftFace), path(path) {
  m_msdfFont = msdfgen::adoptFreetypeFont(m_ftFace);
  m_hbFont = hb_ft_font_create_referenced(m_ftFace);
  setSize(width, height);
}

Font::~Font() {
  if (m_hbFont) {
    hb_free(m_hbFont);
  }
  if (m_ftFace) {
    FT_Done_Face(m_ftFace);
  }
}

std::vector<std::vector<ShapedGlyph>>
Font::shapeText(const std::string &text) const {

  // preprocess text to seperate based on newline character \n and \r

  auto lines = splitString(text, '\n', '\r');
  // for (auto l : lines) {
  //   std::cerr << l << "\n";
  // }

  std::vector<std::vector<ShapedGlyph>> results;
  results.reserve(lines.size());
  for (const auto &line : lines) {

    hb_buffer_t *buffer = hb_buffer_create();
    hb_buffer_add_utf8(buffer, line.c_str(), static_cast<int>(line.size()), 0,
                       static_cast<int>(line.size()));
    hb_buffer_guess_segment_properties(buffer);

    assert(m_hbFont);

    hb_shape(m_hbFont, buffer, nullptr, 0);
    unsigned int count = 0;
    auto *infos = hb_buffer_get_glyph_infos(buffer, &count);
    auto *positions = hb_buffer_get_glyph_positions(buffer, &count);
    std::vector<ShapedGlyph> shapedGlyphs;
    shapedGlyphs.reserve(count);

    for (unsigned int i = 0; i < count; i++) {
      // DK: i don't know why we are dividing by 64.0f
      auto thingy = ShapedGlyph{
          .id = infos[i].codepoint,
          .xAdvance = positions[i].x_advance / 64.0f,
          .yAdvance = positions[i].y_advance / 64.0f,
          .xOffset = positions[i].x_offset / 64.0f,
          .yOffset = positions[i].y_offset / 64.0f,
          // .isNewLine = infos[i].;
      };

      shapedGlyphs.push_back(thingy);
    }

    hb_buffer_destroy(buffer);
    results.push_back(shapedGlyphs);
  }
  // std::cerr << "\n";
  return results;
}

std::vector<msdf_atlas::GlyphGeometry> Font::generateGlyphGeometries() const {
  std::vector<msdf_atlas::GlyphGeometry> glyphs;
  msdf_atlas::FontGeometry geometry(&glyphs);

  assert(m_msdfFont);

  msdf_atlas::Charset glyphset;
  // TODO: I would eventualy like to give the developer option to add only the
  // glyphs that are specified by the user but rn this shoud be fine
  for (unsigned i = 0; i < m_ftFace->num_glyphs; ++i)
    glyphset.add(i);
  auto loaded = geometry.loadGlyphset(m_msdfFont, 1.0, glyphset);

  std::cerr << "loaded: " << loaded << "\n";

  for (auto &glyph : glyphs) {
    glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, 3.0, 0);
  }

  return glyphs;
}
// TODO: havent actually generated glyphs do this
void Font::generateGlyphs(
    const std::vector<msdf_atlas::GlyphGeometry> &glyphGeometries) {
  for (const auto &glyphGeometry : glyphGeometries) {
    glyph g;
    g.id = glyphGeometry.getIndex();
    glyphGeometry.getQuadAtlasBounds(g.atlasLeft, g.atlasTop, g.atlasRight,
                                     g.atlasBottom);
    glyphGeometry.getQuadPlaneBounds(g.left, g.bottom, g.right, g.top);
    m_glyphs.push_back(g);
  }
}

void Font::generateAtlas(
    std::vector<msdf_atlas::GlyphGeometry> &glyphGeometries) {

  msdf_atlas::TightAtlasPacker packer;
  packer.setDimensionsConstraint(msdf_atlas::DimensionsConstraint::SQUARE);
  // this sets the minimum "space a glyph will take" so big number better if
  // viewing letters from close
  packer.setMinimumScale(60.0);
  // how much information needs to be retained outside of an edge basically to
  // know how far i am from an edge, helpful for glowing texts and stuff
  packer.setPixelRange(10.0);
  // couldnt figure out this tho
  // TODO: figure oute what this does (I couldn't see difference between miter
  // of 0.1 and 40, other than 0.1 is a teeny bit blurrier than 40)
  packer.setMiterLimit(1);
  auto error = packer.pack(glyphGeometries.data(), glyphGeometries.size());
  if (error) {
    std::cerr << "error when packing atlas\n";
  }
  int width, height;
  packer.getDimensions(width, height);

  msdf_atlas::ImmediateAtlasGenerator<
      float, 3, msdf_atlas::msdfGenerator,
      msdf_atlas::BitmapAtlasStorage<unsigned char, 3>>
      generator(width, height);
  generator.generate(glyphGeometries.data(), glyphGeometries.size());
  m_atlas = generator.atlasStorage();
  msdfgen::BitmapRef<unsigned char, 3> m = m_atlas;
}

} // namespace Core
