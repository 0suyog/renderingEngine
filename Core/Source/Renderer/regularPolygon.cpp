#include "regularPolygon.h"
#include "Renderer/mesh.h"
#include "glm/trigonometric.hpp"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
namespace Renderer {
RegularPolygon::RegularPolygon(float radius, unsigned int sides,
                               const glm::vec4 &color)
    : sides(sides < 3 ? 3 : sides), radius(radius),
      mat(Material{.fillColor = color}), meshes(generateMesh()) {}

std::vector<Renderer::Mesh> RegularPolygon::generateMesh() {
  float angle = 360.0f / sides;
  float offset = 90 / 2.0;
  if (sides % 2 == 0) {
    float offset = 90;
  }
  std::vector<Renderer::Mesh> meshes;
  std::vector<Renderer::Vertex> vertices;
  std::vector<unsigned int> indices;
  indices.reserve((sides - 2) * 3);

  for (int i = 0; i < sides; i++) {
    Renderer::Vertex v;
    float vertexAngle = glm::radians(offset + angle * i);
    v.position.z = 1.0;
    v.texCoord = {glm::cos(vertexAngle), glm::sin(vertexAngle)};
    v.position.x = radius * v.texCoord.x;
    v.position.y = radius * v.texCoord.y;
    v.normal = {0, 0, 1};
    v.texCoord = (v.texCoord + glm::vec2(1, 1)) * glm::vec2(1 / 2.0);

    vertices.push_back(v);
  }
  // number of triangles required for a polygon is sides - 2
  for (unsigned int i = 0; i < (sides - 2); i++) {
    indices.push_back(0);
    indices.push_back(i + 1);
    indices.push_back(i + 2);
  }
  std::cerr << mat.fillColor->x << " " << mat.fillColor->y << " "
            << mat.fillColor->z << "\n";
  meshes.push_back(Renderer::Mesh(vertices, indices, mat));

  return meshes;
}
} // namespace Renderer
