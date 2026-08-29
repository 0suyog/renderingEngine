#pragma once
#include "Renderer/mesh.h"
#include "glm/ext/vector_float3.hpp"
#include <glm/ext/vector_float2.hpp>
#include <vector>
#include "Renderer/model.h"

namespace Shape {
class Polygon : Renderer::Model {
public:
  Polygon(const glm::vec3 &center, float radius, int sides,
          const glm::vec3 &color);

private:
  std::vector<Renderer::Mesh> generateMesh(float radius, int sides);

private:
  int sides = 3;
  float radius = 1;
  glm::vec3 center = {0, 0, 0};
};
} // namespace Shape
