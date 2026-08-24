#pragma once
#include "glm/ext/matrix_float4x4.hpp"
namespace Core {
class Transform {
public:
  glm::mat4 matrix() const;
  Transform &rotate(float yaw, float pitch, float roll);
  Transform &rotate(float allAxisAngle);
  // void revolve(const glm::vec3 &around, float radius, float angleInDeg,
  //              const glm::vec3 &axis, const glm::vec3 &zerodegDir);
  Transform &moveto(const glm::vec3 &pos);
  Transform &scale(const glm::vec3 &scale);

private:
  glm::vec3 m_rotation = {0, 0, 0};
  glm::vec3 m_position;
  glm::vec3 m_scale;
  glm::mat4 m_transformationMatrix = glm::mat4(1.0f);
};
} // namespace Core
