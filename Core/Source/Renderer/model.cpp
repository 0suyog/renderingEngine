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
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
  static int counter = 1;
  counter++;
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
    if (diffuseTextures.size() > 0) {

      textures[TextureType::DIFFUSE] = diffuseTextures;
    }
    if (specularTextures.size() > 0) {
      textures[TextureType::SPECULAR] = specularTextures;
    }
  }
  return Mesh(vertices, indices, textures);
};

std::vector<std::shared_ptr<Texture>>
Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type) {

  static int counter = 1;
  counter++;
  std::vector<std::shared_ptr<Texture>> returnValue;
  for (int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    returnValue.push_back(TextureFromFile(m_directory / str.C_Str()));
  }
  return returnValue;
}

Model Model::Cube() {
  std::vector<Vertex> vertices = {
      // Front (+Z)
      {{-0.5f, -0.5f, 0.5f}, {0, 0, 1}, {0, 0}},
      {{0.5f, -0.5f, 0.5f}, {0, 0, 1}, {1, 0}},
      {{0.5f, 0.5f, 0.5f}, {0, 0, 1}, {1, 1}},
      {{-0.5f, 0.5f, 0.5f}, {0, 0, 1}, {0, 1}},

      // Back (-Z)
      {{0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0, 0}},
      {{-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1, 0}},
      {{-0.5f, 0.5f, -0.5f}, {0, 0, -1}, {1, 1}},
      {{0.5f, 0.5f, -0.5f}, {0, 0, -1}, {0, 1}},

      // Left (-X)
      {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0, 0}},
      {{-0.5f, -0.5f, 0.5f}, {-1, 0, 0}, {1, 0}},
      {{-0.5f, 0.5f, 0.5f}, {-1, 0, 0}, {1, 1}},
      {{-0.5f, 0.5f, -0.5f}, {-1, 0, 0}, {0, 1}},

      // Right (+X)
      {{0.5f, -0.5f, 0.5f}, {1, 0, 0}, {0, 0}},
      {{0.5f, -0.5f, -0.5f}, {1, 0, 0}, {1, 0}},
      {{0.5f, 0.5f, -0.5f}, {1, 0, 0}, {1, 1}},
      {{0.5f, 0.5f, 0.5f}, {1, 0, 0}, {0, 1}},

      // Top (+Y)
      {{-0.5f, 0.5f, 0.5f}, {0, 1, 0}, {0, 0}},
      {{0.5f, 0.5f, 0.5f}, {0, 1, 0}, {1, 0}},
      {{0.5f, 0.5f, -0.5f}, {0, 1, 0}, {1, 1}},
      {{-0.5f, 0.5f, -0.5f}, {0, 1, 0}, {0, 1}},

      // Bottom (-Y)
      {{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0, 0}},
      {{0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1, 0}},
      {{0.5f, -0.5f, 0.5f}, {0, -1, 0}, {1, 1}},
      {{-0.5f, -0.5f, 0.5f}, {0, -1, 0}, {0, 1}},
  };

  std::vector<unsigned int> indices = {
      0,  1,  2,  0,  2,  3,  // Front
      4,  5,  6,  4,  6,  7,  // Back
      8,  9,  10, 8,  10, 11, // Left
      12, 13, 14, 12, 14, 15, // Right
      16, 17, 18, 16, 18, 19, // Top
      20, 21, 22, 20, 22, 23  // Bottom
  };
  Mesh mesh(vertices, indices);
  return Model(std::vector<Mesh>{std::move(mesh)});
};

Model Model::Quad() {
  std::vector<Vertex> vertices = {
      {{-0.5f, -0.5f, 0.0f}, {0, 0, 1}, {0, 0}},
      {{0.5f, -0.5f, 0.0f}, {0, 0, 1}, {1, 0}},
      {{0.5f, 0.5f, 0.0f}, {0, 0, 1}, {1, 1}},
      {{-0.5f, 0.5f, 0.0f}, {0, 0, 1}, {0, 1}},
  };

  std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

  Mesh mesh(vertices, indices);
  return Model(std::vector<Mesh>{std::move(mesh)});
}

Model Model::Triangle() {
  std::vector<Vertex> vertices = {
      {{-0.5f, -0.5f, 0.0f}, {0, 0, 1}, {0, 0}},
      {{0.5f, -0.5f, 0.0f}, {0, 0, 1}, {1, 0}},
      {{0.0f, 0.5f, 0.0f}, {0, 0, 1}, {0.5f, 1}},
  };

  std::vector<unsigned int> indices = {0, 1, 2};

  Mesh mesh(vertices, indices);
  return Model(std::vector<Mesh>{std::move(mesh)});
}

Model Model::Pyramid(float s, float h) {

  std::vector<Vertex> vertices = {
      // Front
      {{-s, -s, s}, {}, {0, 0}},
      {{s, -s, s}, {}, {1, 0}},
      {{0, h, 0}, {}, {0.5f, 1}},

      // Right
      {{s, -s, s}, {}, {0, 0}},
      {{s, -s, -s}, {}, {1, 0}},
      {{0, h, 0}, {}, {0.5f, 1}},

      // Back
      {{s, -s, -s}, {}, {0, 0}},
      {{-s, -s, -s}, {}, {1, 0}},
      {{0, h, 0}, {}, {0.5f, 1}},

      // Left
      {{-s, -s, -s}, {}, {0, 0}},
      {{-s, -s, s}, {}, {1, 0}},
      {{0, h, 0}, {}, {0.5f, 1}},

      // Bottom
      {{-s, -s, -s}, {0, -1, 0}, {0, 0}},
      {{s, -s, -s}, {0, -1, 0}, {1, 0}},
      {{s, -s, s}, {0, -1, 0}, {1, 1}},
      {{-s, -s, s}, {0, -1, 0}, {0, 1}},
  };

  auto normal = [](glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    return glm::normalize(glm::cross(b - a, c - a));
  };

  // Calculate normals for each sloped face.
  for (int i = 0; i < 12; i += 3) {
    glm::vec3 n = normal(vertices[i].position, vertices[i + 1].position,
                         vertices[i + 2].position);

    vertices[i].normal = n;
    vertices[i + 1].normal = n;
    vertices[i + 2].normal = n;
  }

  std::vector<unsigned int> indices = {0,  1,  2,  // Front
                                       3,  4,  5,  // Right
                                       6,  7,  8,  // Back
                                       9,  10, 11, // Left
                                       12, 13, 14, // Bottom
                                       12, 14, 15};

  Mesh mesh(vertices, indices);
  return Model(std::vector<Mesh>{std::move(mesh)});
}

} // namespace Renderer
