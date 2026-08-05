#pragma once
#include "glm/ext/matrix_float4x4.hpp"
namespace Core {
class Transform {
public:
  float yaw, pitch, roll;
  glm::vec3 position;

  glm::mat4 getTransformationMatrix() const;
  void rotate(float angle_in_deg, const glm::vec3 &axis);
  void revolve(const glm::vec3 &around, float radius, float angleInDeg,
               const glm::vec3 &axis, const glm::vec3 &zerodegDir);
  void moveto(glm::vec3 pos);
  void scale(const glm::vec3 &scale);

private:
  glm::mat4 m_transformationMatrix = glm::mat4(1.0f);
};
} // namespace Core
