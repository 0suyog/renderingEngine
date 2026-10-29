#pragma once
#include "Font/font.h"
#include "Renderer/mesh.h"
#include "Renderer/texture.h"
#include "transform.h"
#include <iostream>
#include <string>

namespace Renderer {

struct RenderableText {
  std::vector<MeshGeometry> meshGeometryes;
  Core::Transform transform;
  glm::vec4 foregroundColor;
  glm::vec4 backgroundColor;
  std::shared_ptr<Texture> atlas;
};

class TextManager {
public:
  void add(const std::shared_ptr<Core::Font> &font, const std::string &string,
           const Core::Transform &transform,
           const glm::vec4 &foregroundColor = {1, 1, 1, 1},
           const glm::vec4 &backgroundColor = {0, 0, 0, 1});
  const std::vector<RenderableText> &getTexts() const { return m_texts; }

private:
  std::vector<RenderableText> m_texts;
};

} // namespace Renderer
