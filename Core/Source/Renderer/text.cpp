#include "text.h"
#include "Font/font.h"

namespace Renderer {
void TextManager::add(const Core::Font &font, const std::string &string,
                      const Core::Transform &transform,
                      const glm::vec4 &foregroundColor,
                      const glm::vec4 &backgroundColor) {
  auto shapedGlyphs = font.shapeText(string);
  auto renderableText = Core::shapedGlyphsToRenderableText(
      font, shapedGlyphs, {0, 0, 1}, transform);
  m_texts.push_back(renderableText);
}

} // namespace Renderer
