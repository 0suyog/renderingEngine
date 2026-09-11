#pragma once
#include "Physics/boundingSphere.h"
#include "Physics/intersectData.h"
#include "glm/ext/vector_float3.hpp"
namespace Physics {
class Plane {
public:
  Plane(const glm::vec3 &normal, float distance)
      : m_normal(normal), m_distance(distance) {}

  const glm::vec3 &getNormal() const { return m_normal; }
  float getDistance() const { return m_distance; }

  Plane Normalized() const;
  IntersectData IntersectBoundingSphere(const BoundingSphere &sphere) const;

private:
  const glm::vec3 m_normal;
  const float m_distance;
};
} // namespace Physics
