#pragma once

#include "shader.h"
namespace Renderer {
class Renderable {
public:
  virtual void render(const ShaderProgram &s) = 0;
};
} // namespace Renderer
