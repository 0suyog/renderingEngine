#include "model.h"
#include "assimp/Importer.hpp"
#include "assimp/cimport.h"
#include "assimp/material.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/types.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
namespace Renderer {
void Model::render(const ShaderProgram &s) const {
  for (const auto &mesh : m_meshes) {
    mesh.render(s);
  }
}

void Model::loadModel(std::filesystem::path path) {
  std::cerr << "inside load model" << "\n";
  Assimp::Importer importer;
  const aiScene *scene =
      importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cerr << "[ASSIMP] error loading model " << path << " "
              << aiGetErrorString() << "\n";
    exit(-1);
  }
  m_directory = path.parent_path();
  std::cerr << "process node now" << "\n";
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  static int counter = 1;
  counter++;
  std::cerr << "ran process node " << counter << "times\n";
  for (int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *assimpMesh = scene->mMeshes[node->mMeshes[i]];
    m_meshes.push_back(processMesh(assimpMesh, scene));
  }
  for (int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  static int counter = 1;
  counter++;
  std::cerr << "ran process mesh " << counter << "times\n";
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::unordered_map<TextureType, std::vector<std::shared_ptr<Texture>>>
      textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex v;
    auto assimpVertices = mesh->mVertices;
    v.position.x = assimpVertices[i].x;
    v.position.y = assimpVertices[i].y;
    v.position.z = assimpVertices[i].z;
    if (mesh->HasNormals()) {
      auto assimpNormals = mesh->mNormals;
      v.normal.x = assimpNormals[i].x;
      v.normal.y = assimpNormals[i].y;
      v.normal.z = assimpNormals[i].z;
    } else {
      v.normal = {0, 1, 0};
    }
    if (mesh->HasTextureCoords(0)) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      v.texCoord = vec;
    } else {
      v.texCoord = {0, 0};
    }
    vertices.push_back(v);
  }
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<std::shared_ptr<Texture>> diffuseTextures =
        loadMaterialTextures(material, aiTextureType_DIFFUSE);
    std::vector<std::shared_ptr<Texture>> specularTextures =
        loadMaterialTextures(material, aiTextureType_SPECULAR);

    textures[TextureType::DIFFUSE] = diffuseTextures;
    textures[TextureType::SPECULAR] = specularTextures;
  }
  return Mesh(vertices, indices, textures);
};

std::vector<std::shared_ptr<Texture>>
Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type) {

  static int counter = 1;
  counter++;
  std::cerr << "ran  loadmaterialtextures " << counter << "times\n";
  std::vector<std::shared_ptr<Texture>> returnValue;
  for (int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    returnValue.push_back(TextureFromFile(m_directory / str.C_Str()));
  }
  return returnValue;
}

} // namespace Renderer
