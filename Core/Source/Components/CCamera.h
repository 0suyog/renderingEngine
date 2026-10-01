#pragma once
#include "Renderer/camera.h"
#include <memory>
namespace Component {
class CCamera {
public:
  CCamera(std::shared_ptr<Renderer::Camera> camera, float followDistance = 10)
      : camera(camera), followDistance(followDistance) {}
  std::shared_ptr<Renderer::Camera> camera;
  float followDistance = 10;
};
} // namespace Component
