#include "transform.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"
namespace Core {

void Transform::rotate(float angle_in_deg, const glm::vec3 &axis) {
  m_transformationMatrix =
      glm::rotate(m_transformationMatrix, glm::radians(angle_in_deg), axis);
}

void Transform::revolve(const glm::vec3 &around, float radius, float angleInDeg,
                        const glm::vec3 &axis, const glm::vec3 &zerodegDir) {
  auto perpTozero = cross(axis, zerodegDir);
  auto rad = glm::radians(angleInDeg);
  auto orbit =
      (zerodegDir * glm::cos(rad) + perpTozero * glm::sin(rad)) * radius;
  m_transformationMatrix =
      glm::translate(m_transformationMatrix, around + orbit);
}

glm::mat4 Transform::getTransformationMatrix() const {
  return m_transformationMatrix;
}

void Transform::scale(const glm::vec3 &scale) {
  m_transformationMatrix = glm::scale(m_transformationMatrix, scale);
}

} // namespace Core
