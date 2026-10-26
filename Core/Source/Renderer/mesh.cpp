#include "mesh.h"
namespace Renderer {

void MeshGeometry::setupMeshGeometry() {

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

Mesh::Mesh(const MeshGeometry &meshGeometry, const Material &mat)
    : m_geometry(meshGeometry), mat(mat) {}

Mesh::Mesh(const MeshGeometry &meshGeometry)
    : m_geometry(meshGeometry), mat() {}
} // namespace Renderer
