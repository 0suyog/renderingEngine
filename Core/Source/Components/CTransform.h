#pragma once
#include "glm/ext/vector_float3.hpp"
#include "transform.h"
namespace Component {
class CTransform {
public:
  glm::vec3 position;
  glm::vec3 velocity;
  float yaw = 0, pitch = 0, roll = 0;
};
} // namespace Component
