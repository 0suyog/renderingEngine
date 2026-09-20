#pragma once

#include "Renderer/mesh.h"
#include "Renderer/regularPolygon.h"
namespace Renderer {
class Shapes3D {
public:
  friend void render(const RenderContext &renderCtx, const Shapes3D &shape,
                     const glm::mat4 &modelMatrix);

private:
  RegularPolygon polygon;
  Material mat;
  int m_polygonNumber;
};
} // namespace Renderer
