#include "Physics/aabb.h"
#include "Physics/intersectData.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
namespace Physics {
IntersectData Aabb::IntersectAABB(const Aabb &other) {
  auto distance1 = other.GetMinExtents() - m_maxExtents;
  auto distance2 = m_minExtents - other.GetMaxExtents();
  auto maxDistances = (glm::max(distance1, distance2));
  auto maxDistance =
      std::max(std::max(maxDistances.x, maxDistances.y), maxDistances.z);
  return IntersectData(maxDistance < 0, maxDistance);
}
} // namespace Physics
