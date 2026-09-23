#pragma once
#include "Renderer/camera.h"
#include <memory>
namespace Renderer {
struct RenderContext {
  std::shared_ptr<Camera> cam;
  float viewPortWidth;
  float viewPortHeight;
};
} // namespace Renderer
