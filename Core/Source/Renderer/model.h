#pragma once
#include "assimp/scene.h"
#include "mesh.h"
#include <filesystem>
#include <memory>

namespace Renderer {
class Model {
public:
  Model(const std::filesystem::path &path) { loadModel(path); }
  Model(const std::vector<Mesh> &m_meshes) : m_meshes(m_meshes) {}

  friend void render(const RenderContext &renderCtx, const Model &model,
                     const glm::mat4 &modelMatrix);

private:
  // model data
  std::vector<Mesh> m_meshes;
  std::filesystem::path m_directory;
  void loadModel(const std::filesystem::path &path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<std::shared_ptr<Texture>>
  loadMaterialTextures(aiMaterial *mat, aiTextureType type);
};
} // namespace Renderer
