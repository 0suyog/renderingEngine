// #pragma once
// #include <glad/glad.h>
// #include <filesystem>
//
// namespace Renderer {
// struct Texture {
//   GLuint Handle = 0;
//   uint32_t Width = 0;
//   uint32_t Height = 0;
// };
//
// struct Framebuffer {
//   GLuint Handle = 0;
//   Texture ColorAttachment;
// };
//
// Texture CreateTexture(int width, int height);
// Texture LoadTexture(const std::filesystem::path &path);
// Framebuffer CreateFramebufferWithTexture(const Texture texture);
// bool AttachTextureToFramebuffer(Framebuffer &framebuffer,
//                                 const Texture texture);
// void BlitFramebufferToSwapchain(const Framebuffer framebuffer);
//
// } // namespace Renderer
#pragma once
#include "Renderer/mesh.h"
#include "Renderer/model.h"
#include "Renderer/renderContext.h"
#include "Renderer/shader.h"
#include "Renderer/regularPolygon.h"
namespace Renderer {

void render(const RenderContext &renderCtx, const Mesh &mesh,
            const glm::mat4 &modelMatrix, const ShaderProgram &s);

void render(const RenderContext &renderCtx, const Model &model,
            const glm::mat4 &modelMatrix);

void render(const RenderContext &renderCtx, const RegularPolygon &shape,
            const glm::mat4 &modelMatrix);

} // namespace Renderer
