#include "text.h"

namespace Renderer {
void TextManager::add(const Core::Font &font, const std::string &string,
                      const Core::Transform &transform,
                      const glm::vec4 &foregroundColor,
                      const glm::vec4 &backgroundColor) {
  auto shapedGlyphs = font.shapeText(string);
}

} // namespace Renderer
