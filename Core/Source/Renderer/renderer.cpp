#include "light.h"
#include "Renderer/mesh.h"
#include "Renderer/model.h"
#include "Renderer/renderContext.h"
#include "Renderer/shader.h"
#include "Renderer/shapes3d.h"
#include "regularPolygon.h"
#include "Renderer/texture.h"
#include <iostream>
#include <string>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
namespace Renderer {

/**
 * @brief Sends light to shader through uniforms
 *
 * @param s The shader program to use
 * @param l Light to be sent
 * @param index Index of light
 */
void sendLightToShader(ShaderProgram s, const Light &l, unsigned int index) {
  const std::string prefix = "lights[" + std::to_string(index) + "].";

  s.SetVec3(prefix + "ambient", l.ambient);
  s.SetVec3(prefix + "diffuse", l.diffuse);
  s.SetVec3(prefix + "specular", l.specular);

  s.SetVec3(prefix + "pos", l.pos);
  s.SetVec3(prefix + "dir", l.dir);

  s.SetFloat(prefix + "cutoff", l.cutoff);
  s.SetFloat(prefix + "outerCutoff", l.outerCutoff);

  s.SetFloat(prefix + "constant", l.constant);
  s.SetFloat(prefix + "linear", l.linear);
  s.SetFloat(prefix + "quadratic", l.quadratic);

  s.SetUInt(prefix + "type", static_cast<unsigned int>(l.type));
}

void sendLightsToShader(ShaderProgram s, std::vector<Light> lights) {
  for (unsigned int i = 0; i < lights.size(); i++) {
    sendLightToShader(s, lights[i], i);
  }
}

void render(const RenderContext &renderCtx, const Mesh &mesh,
            const glm::mat4 &modelMatrix, const ShaderProgram &s) {
  glViewport(0, 0, renderCtx.viewPortWidth, renderCtx.viewPortHeight);
  s.use();
  s.SetMat4("view", renderCtx.cam->lookAt());
  s.SetMat4("projection", renderCtx.cam->prespectiveMat());
  s.SetMat4("model", modelMatrix);
  s.SetVec3("CameraPosition", renderCtx.cam->getPosition());
  std::shared_ptr<Texture> diffuseTex, specularTex;
  glm::vec4 color = {1, 1, 1, 1.0};
  // check if matrial has fillColor or diffuseTexture
  // if none then just diffuse texture other wise
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
    specularTex = GetBuiltInTexture(BuiltInTexture::BLACK);
  }
  s.SetFloat("material.shininess", mesh.mat.shininess);
  glActiveTexture(GL_TEXTURE0);
  s.SetInt("material.diffuse", 0);
  glBindTexture(GL_TEXTURE_2D, diffuseTex->handle);
  s.SetVec4("material.fillColor", color);
  glActiveTexture(GL_TEXTURE1);
  s.SetInt("material.specular", 1);
  glBindTexture(GL_TEXTURE_2D, specularTex->handle);
  if (mesh.mat.normalTexture) {
    glActiveTexture(GL_TEXTURE2);
    s.SetInt("material.normal", 2);
    glBindTexture(GL_TEXTURE_2D, mesh.mat.normalTexture.value()->handle);
    s.SetInt("material.hasNormal", 1);
  } else {
    s.SetInt("material.hasNormal", 0);
  }

  glBindVertexArray(mesh.m_VAO);
  // for wireframe
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
}

void render(const RenderContext &renderCtx, const Model &model,
            const glm::mat4 &modelMatrix) {
  for (const auto &mesh : model.m_meshes) {
    render(renderCtx, mesh, modelMatrix, *getShader(EShaders::PhongShader));
  }
}

void render(const RenderContext &renderCtx, const RegularPolygon &shape,
            const glm::mat4 &modelMatrix) {
  for (const auto &mesh : shape.meshes) {
    render(renderCtx, mesh, modelMatrix, *getShader(EShaders::PhongShader));
  }
}

void render(const RenderContext &renderCtx, const Shapes3D &shape,
            const glm::mat4 &modelMatrix) {
  auto shader = getShader(EShaders::PhongShader);
  sendLightsToShader(*shader, renderCtx.lights);
  for (const auto &mesh : shape.m_meshes) {
    render(renderCtx, mesh, modelMatrix, *shader);
  }
}

// static Text text;

// void render(const RenderContext &renderCtx, const std::string &str,
//             glm::mat4 modelMatrix) {
//   auto shader = getShader(EShaders::TEXTSHADER);
//   // auto shapedText = text.shapeText(str);
//   float penX = 0.0f;
//   float penY = 0.0f;
//   // for (const auto &st : shapedText) {
//   //   auto w = fabs(st.xAdvance);
//   //   auto h = fabs(st.yAdvance);
//   //   auto s = Renderer::RegularPolygon(fmax(w, h), 4);
//   //   auto localTransform = Core::Transform();
//   //   localTransform.setPosition({penX + st.xOffset, st.yOffset, 0});
//   // }
// }

} // namespace Renderer
