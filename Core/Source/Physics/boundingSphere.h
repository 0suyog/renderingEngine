#pragma once
#include "Physics/intersectData.h"
#include "glm/ext/vector_float3.hpp"
namespace Physics {
class BoundingSphere {
public:
  BoundingSphere(glm::vec3 center, float radius)
      : m_center(center), m_radius(radius) {}
  IntersectData IntersectBoundingSphere(const BoundingSphere &other);
  float Getradius() const { return m_radius; };
  inline const glm::vec3 &GetCenter() const { return m_center; }

private:
  const glm::vec3 m_center;
  const float m_radius;
};
} // namespace Physics
