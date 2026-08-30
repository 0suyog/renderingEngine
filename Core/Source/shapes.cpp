#include "shapes.h"
#include "Renderer/mesh.h"
#include "Renderer/model.h"
#include "glm/trigonometric.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include <vector>
namespace Shape {
Polygon::Polygon(const glm::vec3 &center, float radius, unsigned int sides,
                 const glm::vec3 &color = {0.38f, 0.6f, 0.89f})
    : Renderer::Model(generateMesh(radius, sides < 3 ? 3 : sides)),
      sides(sides < 3 ? 3 : sides), radius(radius), center(center) {}

std::vector<Renderer::Mesh> Polygon::generateMesh(float radius,
                                                  unsigned int sides) {
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
    v.position.z = 0;
    v.texCoord = {glm::cos(vertexAngle), glm::sin(vertexAngle)};
    std::cerr << vertexAngle << "\n";
    std::cerr << "texCoord: " << glm::to_string(v.texCoord) << "\n";
    std::cerr << "cos angle: " << glm::cos(vertexAngle)
              << "sin angle: " << glm::sin(vertexAngle) << "\n";

    v.position.x = radius * v.texCoord.x;
    v.position.y = radius * v.texCoord.y;
    v.normal = {0, 0, 1};
    vertices.push_back(v);
  }
  // number of triangles required for a polygon is sides - 2
  for (unsigned int i = 0; i < (sides - 2); i++) {
    indices.push_back(0);
    indices.push_back(i + 1);
    indices.push_back(i + 2);
  }

  meshes.push_back(Renderer::Mesh(vertices, indices));

  return meshes;
}

} // namespace Shape
