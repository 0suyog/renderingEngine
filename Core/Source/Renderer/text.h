#pragma once
#include "Font/font.h"
#include "Renderer/mesh.h"
#include "transform.h"
#include <string>

namespace Renderer {

struct RenderableText {
  std::vector<Mesh> meshes;
  Core::Transform transform;
  glm::vec4 foregroundColor;
  glm::vec4 backgroundColor;
};

class TextManager {
public:
  void add(const Core::Font &font, const std::string &string,
           const Core::Transform &transform,
           const glm::vec4 &foregroundColor = {1, 1, 1, 1},
           const glm::vec4 &backgroundColor = {0, 0, 0, 1});

private:
  std::vector<RenderableText> m_texts;
};

} // namespace Renderer
