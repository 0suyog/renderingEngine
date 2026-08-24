#pragma once
#include "glm/ext/matrix_float4x4.hpp"
namespace Core {
class Transform {
public:
  float getPitch() const;
  float getYaw() const;
  float getRoll() const;

  const glm::vec3 &getPosition() const;
  const glm::vec3 &getScale() const;

  glm::mat4 matrix() const;
  Transform &rotate(float pitch, float yaw, float roll);
  Transform &rotate(float allAxisAngle);
  // void revolve(const glm::vec3 &around, float radius, float angleInDeg,
  //              const glm::vec3 &axis, const glm::vec3 &zerodegDir);
  Transform &moveto(const glm::vec3 &pos);
  Transform &scale(const glm::vec3 &scale);

private:
  // pitch, yaw, roll
  glm::vec3 m_rotation = {0.0f, 0.0f, 0.0f};
  glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
  glm::vec3 m_scale = {1.0f, 1.0f, 1.0f};
};
} // namespace Core
