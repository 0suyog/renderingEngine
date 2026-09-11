#pragma once
#include "Physics/intersectData.h"
#include "glm/ext/vector_float3.hpp"
namespace Physics {
class Aabb {
public:
  Aabb(glm::vec3 minExtents, glm::vec3 maxExtents)
      : m_minExtents(minExtents), m_maxExtents(maxExtents) {}

  IntersectData IntersectAABB(const Aabb &other);

  const glm::vec3 &GetMinExtents() const { return m_minExtents; }

  const glm::vec3 &GetMaxExtents() const { return m_maxExtents; }

private:
  const glm::vec3 m_minExtents;
  const glm::vec3 m_maxExtents;
};
} // namespace Physics
