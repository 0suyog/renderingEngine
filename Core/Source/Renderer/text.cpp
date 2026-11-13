#include "text.h"
#include "Font/font.h"
#include <iostream>

namespace Renderer {
void TextManager::add(const std::shared_ptr<Core::Font> &font,
                      const std::string &string,
                      const Core::Transform &transform,
                      const glm::vec4 &foregroundColor,
                      const glm::vec4 &backgroundColor) {

  font->getAtlasTexture();
  auto shapedLines = font->shapeText(string);
  auto renderableText = Core::shapedGlyphsToRenderableText(
      font, shapedLines, {0, 0, 1}, transform, foregroundColor,
      backgroundColor);
  m_texts.push_back(renderableText);
}

} // namespace Renderer
