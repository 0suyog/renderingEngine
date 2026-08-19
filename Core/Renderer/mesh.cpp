#include "shader.h"
#include "texture.h"
#include <iostream>
#include <memory>
#include <mesh.h>
#include <string>
#include <sys/types.h>
namespace Renderer {
Mesh::Mesh(
    const std::vector<Vertex> &vertices,
    const std::vector<unsigned int> &indices,
    const std::unordered_map<TextureType, std::vector<std::shared_ptr<Texture>>>
        &textures_map)
    : vertices(vertices), indices(indices), textures_map(textures_map) {
  setupMesh();
}

void Mesh::setupMesh() {
  glCreateBuffers(1, &m_VBO);
  glNamedBufferData(m_VBO, vertices.size() * sizeof(Vertex), &vertices[0],
                    GL_STATIC_DRAW);
  glCreateBuffers(1, &m_EBO);
  glNamedBufferData(m_EBO, indices.size() * sizeof(unsigned int), &indices[0],
                    GL_STATIC_DRAW);
  glCreateVertexArrays(1, &m_VAO);
  glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, sizeof(Vertex));
  // for position
  glEnableVertexArrayAttrib(m_VAO, 0);
  glVertexArrayAttribFormat(m_VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
  glVertexArrayAttribBinding(m_VAO, 0, 0);
  // for normal
  glEnableVertexArrayAttrib(m_VAO, 1);
  glVertexArrayAttribFormat(m_VAO, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
  glVertexArrayAttribBinding(m_VAO, 1, 0);
  // for texCoords
  glEnableVertexArrayAttrib(m_VAO, 2);
  glVertexArrayAttribFormat(m_VAO, 2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float));
  glVertexArrayAttribBinding(m_VAO, 2, 0);
  glVertexArrayElementBuffer(m_VAO, m_EBO);
}

void Mesh::render(const ShaderProgram &s) const {
  s.use();
  GLint activeTexture = 0;
  bool hasDiffuse = false;
  bool hasSpecular = false;
  std::string diffuseName = "diffuse", specularName = "specular";
  for (auto &[type, textures] : textures_map) {
    std::string name;
    switch (type) {
    case TextureType::DIFFUSE:
      std::cerr << textures.size();
      name = diffuseName;
      hasDiffuse = true;
      break;
    case TextureType::SPECULAR:
      name = specularName;
      hasSpecular = true;
      break;
    }
    for (int i = 0; i < textures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 + activeTexture++);
      s.SetInt(("material." + name + std::to_string(i)).c_str(), activeTexture);
      glBindTexture(GL_TEXTURE_2D, textures[i]->handle);
    }
  }
  if (!hasDiffuse) {
    glActiveTexture(GL_TEXTURE0 + activeTexture++);
    s.SetInt(("material." + diffuseName + std::to_string(0)).c_str(),
             activeTexture);
    glBindTexture(GL_TEXTURE_2D,
                  GetBuiltInTexture(BuiltInTexture::DEFAULT)->handle);
  }
  if (!hasSpecular) {
    glActiveTexture(GL_TEXTURE0 + activeTexture++);
    s.SetInt(("material." + specularName + std::to_string(0)).c_str(),
             activeTexture);
    glBindTexture(GL_TEXTURE_2D,
                  GetBuiltInTexture(BuiltInTexture::BLACK)->handle);
  }
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

} // namespace Renderer
