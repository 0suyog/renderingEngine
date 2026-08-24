#include "transform.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
namespace Core {

Transform &Transform::rotate(float yaw, float pitch, float roll) {
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
  mat = glm::rotate(mat, m_rotation.x, {0, 1, 0});
  mat = glm::scale(mat, m_scale);

  return mat;
}

Transform &Transform::scale(const glm::vec3 &scale) {
  m_scale = scale;
  return *this;
}

Transform &Transform::moveto(const glm::vec3 &pos) {
  m_position = pos;
  return *this;
}

} // namespace Core
