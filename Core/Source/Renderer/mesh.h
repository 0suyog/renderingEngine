#pragma once
#include "Renderer/renderContext.h"
#include "Renderer/shader.h"
#include "Renderer/texture.h"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include <memory>
#include <optional>
namespace Renderer {

class Vertex {
public:
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;
};

struct Material {
  std::optional<glm::vec4> fillColor;
  std::optional<std::shared_ptr<Texture>> diffuseTexture;
  std::optional<std::shared_ptr<Texture>> specularTexture;
  std::optional<std::shared_ptr<Texture>> normalTexture;
  float shininess = 128;
};

class Mesh;

class MeshGeometry {
public:
  MeshGeometry(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
      : vertices(vertices), indices(indices) {
    setupMeshGeometry();
  }

  friend void render(const RenderContext &renderCtx, const MeshGeometry &mesh,
                     const glm::mat4 &modelMatrix);

  const std::vector<Vertex> &getVertices() const { return vertices; }
  const std::vector<unsigned int> &getIndiges() const { return indices; }

private:
  void setupMeshGeometry();
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  unsigned int m_VAO, m_VBO, m_EBO;
};

class Mesh {
public:
  Material mat;

  Mesh(const MeshGeometry &meshGeometry, const Material &mat);
  Mesh(const MeshGeometry &meshGeometry);

  const MeshGeometry &getMeshGeometry() const { return m_geometry; }

  friend void render(const RenderContext &renderCtx, const Mesh &mesh,
                     const glm::mat4 &modelMatrix);

private:
  MeshGeometry m_geometry;
};
} // namespace Renderer
