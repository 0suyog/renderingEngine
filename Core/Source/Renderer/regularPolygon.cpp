#include "regularPolygon.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
namespace Renderer {
RegularPolygon::RegularPolygon(float radius, unsigned int sides,
                               const glm::vec4 &color, float startingAngle,
                               const glm::vec3 &normal)
    : sides(sides < 3 ? 3 : sides), radius(radius),
      mat(Material{.fillColor = color}), startingAngle(startingAngle),
      normal(glm::normalize(normal)), meshes(generateMesh()) {}

RegularPolygon::RegularPolygon(float radius, unsigned int sides,
                               const Material &mat, float startingAngle,
                               const glm::vec3 &normal)
    : sides(sides < 3 ? 3 : sides), radius(radius), mat(mat),
      startingAngle(startingAngle), normal(glm::normalize(normal)),
      meshes(generateMesh()) {}

std::vector<Renderer::Mesh> RegularPolygon::generateMesh() {
  float angle = 360.0f / sides;
  float offset = 90.0 - (180.0 / sides);
  if (sides % 2 == 1) {
    offset += 180.0;
  }

  glm::vec3 basisU, basisV, ref = {0, -1, 0};

  if (glm::dot(ref, normal) < 0.999) {
    ref = {-1, 0, 0};
  }

  glm::vec3 reference = glm::vec3(0, 1, 0);

  if (glm::abs(glm::dot(normal, reference)) > 0.999f) {
    reference = glm::vec3(1, 0, 0);
  }

  basisU = glm::normalize(glm::cross(reference, normal));
  basisV = glm::cross(normal, basisU);
  std::vector<Renderer::Mesh> meshes;
  std::vector<Renderer::Vertex> vertices;
  std::vector<unsigned int> indices;
  indices.reserve((sides - 2) * 3);

  for (int i = 0; i < sides; i++) {
    Renderer::Vertex v;
    float vertexAngle = glm::radians(startingAngle + offset + angle * i);
    v.texCoord = {glm::cos(vertexAngle), glm::sin(vertexAngle)};

    auto wcomponent = 1.0f * normal;
    auto ucomponent = radius * v.texCoord.x * basisU;
    auto vcomponent = radius * v.texCoord.y * basisV;
    v.position = wcomponent + ucomponent + vcomponent;
    v.normal = normal;
    v.texCoord = (v.texCoord + glm::vec2(1, 1)) * glm::vec2(1 / 2.0);

    vertices.push_back(v);
  }
  // number of triangles required for a polygon is sides - 2
  for (unsigned int i = 0; i < (sides - 2); i++) {
    indices.push_back(0);
    indices.push_back(i + 1);
    indices.push_back(i + 2);
  }
  meshes.push_back(Renderer::Mesh(vertices, indices, mat));

  return meshes;
}
} // namespace Renderer
