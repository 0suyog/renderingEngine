#pragma once
#include "glm/ext/vector_float3.hpp"
#include "renderables.h"
#include "texture.h"
#include <vector>
namespace Renderer {

class Vertex {
public:
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;
};

class Mesh : public Renderable {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  // std::vector<std::shared_ptr<Texture>> textures;
  std::unordered_map<TextureType, std::vector<std::shared_ptr<Texture>>>
      textures_map;

  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<unsigned int> &indices,
       const std::unordered_map<
           TextureType, std::vector<std::shared_ptr<Texture>>> &textures_map);

  void setupMesh();

  void render(const ShaderProgram &s) const override;

private:
  unsigned int m_VAO, m_VBO, m_EBO;
};
} // namespace Renderer
