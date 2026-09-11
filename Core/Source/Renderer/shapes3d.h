#pragma once

#include "Renderer/mesh.h"
namespace Renderer {
class Shapes3D {

  Shapes3D(float radius, unsigned int polygonSides,
           unsigned int polygonNumber = 1,
           const glm::vec4 &color = {0.38f, 0.6f, 0.89f, 1.0f})
      : m_radius(radius), m_polygonSides(polygonSides),
        m_polygonNumber(polygonNumber), mat(Material{.fillColor = color}),
        m_meshes(generateMeshes()) {}

private:
  std::vector<Mesh> generateMeshes();

  Material mat;
  float m_radius;
  int m_polygonSides;
  int m_polygonNumber;
  std::vector<Mesh> m_meshes;
};
} // namespace Renderer
