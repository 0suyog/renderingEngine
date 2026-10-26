#pragma once
#include "Font/font.h"
#include "Renderer/mesh.h"
#include "transform.h"
#include <string>

namespace Renderer {

class RenderableText {
  std::vector<Mesh> m_meshes;
  Core::Transform transform;
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
