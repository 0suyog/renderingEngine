#pragma once
#include "Renderer/mesh.h"
#include "glm/ext/vector_float3.hpp"
#include <glm/ext/vector_float2.hpp>
#include <vector>
namespace Renderer {

class RegularPolygon {
public:
  RegularPolygon(float radius, unsigned int sides,
                 const glm::vec4 &color = {0.38f, 0.6f, 0.89f, 1.0f});
  friend void render(const RenderContext &renderCtx,
                     const RegularPolygon &shape, const glm::mat4 &modelMatrix);
  Material mat;

private:
  std::vector<Renderer::Mesh> generateMesh();

private:
  unsigned int sides = 3;
  float radius = 1;
  std::vector<Renderer::Mesh> meshes;
};

} // namespace Renderer
