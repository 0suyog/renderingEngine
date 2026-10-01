#pragma once
#include "Renderer/light.h"
#include "Renderer/camera.h"
#include <memory>
namespace Renderer {
struct RenderContext {
  std::shared_ptr<Camera> cam;
  float viewPortWidth;
  float viewPortHeight;
  std::vector<Renderer::Light> lights;
};
} // namespace Renderer
