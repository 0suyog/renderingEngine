#pragma once

#include "Renderer/mesh.h"
#include "Renderer/regularPolygon.h"
namespace Renderer {
class Shapes3D {
public:
  Shapes3D(float radius, unsigned int polygonSides,
           unsigned int polygonNumber = 1,
           const glm::vec4 &color = {0.38f, 0.6f, 0.89f, 1.0f})
      : m_polygonNumber(polygonNumber), mat(Material{.fillColor = color}),
        polygon(RegularPolygon(radius, polygonSides, color)) {}

  friend void render(const RenderContext &renderCtx, const Shapes3D &shape,
                     const glm::mat4 &modelMatrix);

private:
  RegularPolygon polygon;
  Material mat;
  int m_polygonNumber;
};
} // namespace Renderer
