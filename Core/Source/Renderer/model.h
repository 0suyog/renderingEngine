#pragma once
#include "assimp/scene.h"
#include "renderables.h"
#include "shader.h"
#include "mesh.h"
#include <filesystem>
#include <memory>

namespace Renderer {
class Model : public Renderable {
public:
  Model(const std::string &path) { loadModel(path); }
  Model(const std::vector<Mesh> &m_meshes) : m_meshes(m_meshes) {}
  void render(const ShaderProgram &s) const override;

  static Model Cube();
  static Model Triangle();
  static Model Quad();
  static Model Pyramid(float s = 0.5f, float h = 1.0f);

private:
  // model data
  std::vector<Mesh> m_meshes;
  std::filesystem::path m_directory;
  void loadModel(std::filesystem::path path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<std::shared_ptr<Texture>>
  loadMaterialTextures(aiMaterial *mat, aiTextureType type);
};
} // namespace Renderer
