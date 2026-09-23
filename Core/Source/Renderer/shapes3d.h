#pragma once

#include "Renderer/mesh.h"
namespace Renderer {
class Shapes3D {
public:
  Shapes3D(const std::vector<Mesh> &meshes, const Material &mat)
      : m_mat(mat), m_meshes(meshes) {}
  friend void render(const RenderContext &renderCtx, const Shapes3D &shape,
                     const glm::mat4 &modelMatrix);

  Material m_mat;
  std::vector<Mesh> m_meshes;
};

Shapes3D Cube(float sideLen,
              const glm::vec4 &color = {0.38f, 0.6f, 0.89f, 1.0f});
Shapes3D Cube(float sideLen, const Material &mat);
Shapes3D Cuboid(float xLen, float yLen, float zLen,
                const glm::vec4 &color = {0.38f, 0.6f, 0.89f, 1.0f});
Shapes3D Cuboid(float xLen, float yLen, float zLen, const Material &mat);

} // namespace Renderer
