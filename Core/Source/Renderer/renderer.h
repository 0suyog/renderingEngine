#pragma once
#include "Renderer/mesh.h"
#include "Renderer/model.h"
#include "Renderer/renderContext.h"
#include "Renderer/regularPolygon.h"
#include "Renderer/shapes3d.h"
#include "Renderer/text.h"
namespace Renderer {

void render(const RenderContext &renderCtx, const MeshGeometry &meshGeometry,
            const glm::mat4 &modelMatrix);

void render(const RenderContext &renderCtx, const Mesh &mesh,
            const glm::mat4 &modelMatrix);

void render(const RenderContext &renderCtx, const Model &model,
            const glm::mat4 &modelMatrix);

void render(const RenderContext &renderCtx, const RegularPolygon &shape,
            const glm::mat4 &modelMatrix);

void render(const RenderContext &renderCtx, const Shapes3D &shape,
            const glm::mat4 &modelMatrix);

void render(const RenderContext &renderCtx, const RenderableText &text,
            const glm::mat4 &modelMatrix);

} // namespace Renderer
