#pragma once

#include "shader.h"
namespace Renderer {
class Renderable {
public:
  virtual void render(const ShaderProgram &s) const = 0;
};
} // namespace Renderer
