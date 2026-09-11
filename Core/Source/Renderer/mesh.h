#pragma once
#include "Renderer/renderContext.h"
#include "Renderer/shader.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/ext/vector_float4.hpp"
#include "texture.h"
#include <memory>
#include <optional>
#include <vector>
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
  float specularStrength = 0;
};

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  Material mat;

  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<unsigned int> &indices, const Material &mat);

  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<unsigned int> &indices);

  void setupMesh();

  friend void render(const RenderContext &renderCtx, const Mesh &mesh,
                     const glm::mat4 &modelMatrix, const ShaderProgram &s);

private:
  unsigned int m_VAO, m_VBO, m_EBO;
};
} // namespace Renderer
