#include "transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <iostream>
namespace Core {

float Transform::getPitch() const { return m_rotation.x; }

float Transform::getYaw() const { return m_rotation.y; }

float Transform::getRoll() const { return m_rotation.z; }

const glm::vec3 &Transform::getPosition() const { return m_position; }

const glm::vec3 &Transform::getScale() const { return m_scale; }

Transform &Transform::rotate(float pitch, float yaw, float roll) {
  m_rotation.x = pitch;
  m_rotation.y = yaw;
  m_rotation.z = roll;
  return *this;
}

Transform &Transform::rotate(float allAxisAngle) {
  m_rotation = glm::vec3(allAxisAngle);
  return *this;
}

// void Transform::revolve(const glm::vec3 &around, float radius, float
// angleInDeg,
//                         const glm::vec3 &axis, const glm::vec3 &zerodegDir) {
//   auto perpTozero = cross(axis, zerodegDir);
//   auto rad = glm::radians(angleInDeg);
//   auto orbit =
//       (zerodegDir * glm::cos(rad) + perpTozero * glm::sin(rad)) * radius;
//   m_transformationMatrix =
//       glm::translate(m_transformationMatrix, around + orbit);
// }

glm::mat4 Transform::matrix() const {
  glm::mat4 mat = glm::mat4(1.0);
  mat = glm::translate(mat, m_position);
  mat = glm::rotate(mat, m_rotation.x, {1, 0, 0});
  mat = glm::rotate(mat, m_rotation.y, {0, 1, 0});
  mat = glm::rotate(mat, m_rotation.z, {0, 0, 1});
  mat = glm::scale(mat, m_scale);

  return mat;
}

Transform &Transform::scale(const glm::vec3 &scale) {
  m_scale = scale;
  return *this;
}

Transform &Transform::scale(float scale) {
  return this->scale(glm::vec3(scale));
}

Transform &Transform::moveto(const glm::vec3 &pos) {
  m_position = pos;
  return *this;
}

} // namespace Core
