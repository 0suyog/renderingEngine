#pragma once

#include <Renderer/light.h>
namespace Component {
class CLight {
public:
  CLight(const Renderer::Light light) : light(light) {}
  Renderer::Light light;
};
} // namespace Component
