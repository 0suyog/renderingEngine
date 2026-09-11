#include "Renderer/mesh.h"
#include "Renderer/model.h"
#include "Renderer/shader.h"
#include "regularPolygon.h"
#include "Renderer/texture.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
namespace Renderer {
void render(const RenderContext &renderCtx, const Mesh &mesh,
            const glm::mat4 &modelMatrix, const ShaderProgram &s) {
  s.use();
  s.SetMat4("view", renderCtx.cam->lookAt());
  s.SetMat4("projection", renderCtx.cam->orthographicMat());
  s.SetMat4("model", modelMatrix);
  std::shared_ptr<Texture> diffuseTex;
  glm::vec4 color = {1, 1, 1, 1.0};
  std::shared_ptr<Texture> specularTex;
  float specularStrength = mesh.mat.specularStrength;
  // check if matrial has fillColor or diffuseTexture
  // if none then just diffuse texture other wise set
  // check if it has diffuseTexture and if not set it to
  // white 1x1 texture
  if (!mesh.mat.fillColor && !mesh.mat.diffuseTexture) {
    diffuseTex = GetBuiltInTexture(BuiltInTexture::DEFAULT);
  } else {
    if (mesh.mat.diffuseTexture) {
      diffuseTex = mesh.mat.diffuseTexture.value();
    } else {
      diffuseTex = GetBuiltInTexture(BuiltInTexture::WHITE);
    }

    if (mesh.mat.fillColor) {
      color = mesh.mat.fillColor.value();
    }
  }

  if (mesh.mat.specularTexture) {
    specularTex = mesh.mat.specularTexture.value();
  } else {
    specularTex = GetBuiltInTexture(BuiltInTexture::WHITE);
  }

  glActiveTexture(GL_TEXTURE0);
  s.SetInt("material.diffuse", 0);
  glBindTexture(GL_TEXTURE_2D, diffuseTex->handle);
  s.SetVec4("material.fillColor", color);
  glActiveTexture(GL_TEXTURE1);
  s.SetInt("material.specular", 1);
  glBindTexture(GL_TEXTURE_2D, specularTex->handle);
  s.SetFloat("material.specularStrength", specularStrength);
  if (mesh.mat.normalTexture) {
    glActiveTexture(GL_TEXTURE2);
    s.SetInt("material.normal", 2);
    glBindTexture(GL_TEXTURE_2D, mesh.mat.normalTexture.value()->handle);
    s.SetInt("material.hasNormal", 1);
  } else {
    s.SetInt("material.hasNormal", 0);
  }

  glBindVertexArray(mesh.m_VAO);
  glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
}

void render(const RenderContext &renderCtx, const Model &model,
            const glm::mat4 &modelMatrix) {
  for (const auto &mesh : model.m_meshes) {
    render(renderCtx, mesh, modelMatrix, *getShader(EShaders::BasicShader));
  }
}

void render(const RenderContext &renderCtx, const RegularPolygon &shape,
            const glm::mat4 &modelMatrix) {
  for (const auto &mesh : shape.meshes) {
    render(renderCtx, mesh, modelMatrix, *getShader(EShaders::BasicShader));
  }
}

} // namespace Renderer
