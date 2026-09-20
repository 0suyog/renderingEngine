#include "Physics/collider.h"
#include "Physics/boundingSphere.h"
#include "Physics/intersectData.h"
#include "intersections.h"
#include <cstdlib>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

namespace Physics {
IntersectData Collider::Intersect(const Collider &colliderA,
                                  const Collider &colliderB) {
  if (colliderA.m_type == SPHERE && colliderB.m_type == SPHERE) {
    auto &self = static_cast<const BoundingSphere &>(colliderA);
    auto &other = static_cast<const BoundingSphere &>(colliderB);
    return IntersectSphereSphere(self, other);
  }

  if (colliderA.m_type == SPHERE && colliderB.m_type == AABB) {
    auto &self = static_cast<const BoundingSphere &>(colliderA);
    auto &other = static_cast<const Aabb &>(colliderB);
    return IntersectSphereAabb(self, other);
  }

  if (colliderA.m_type == SPHERE && colliderB.m_type == PLANE) {
    auto &self = static_cast<const BoundingSphere &>(colliderA);
    auto &other = static_cast<const Plane &>(colliderB);
    return IntersectSpherePlane(self, other);
  }

  if (colliderA.m_type == AABB && colliderB.m_type == SPHERE) {
    auto &self = static_cast<const Aabb &>(colliderA);
    auto &other = static_cast<const BoundingSphere &>(colliderB);
    return IntersectAabbSphere(self, other);
  }

  if (colliderA.m_type == AABB && colliderB.m_type == AABB) {
    auto &self = static_cast<const Aabb &>(colliderA);
    auto &other = static_cast<const Aabb &>(colliderB);
    std::cerr << "self position: " << glm::to_string(self.GetCenter()) << "\n";
    std::cerr << "other position: " << glm::to_string(other.GetCenter())
              << "\n\n";
    return IntersectAabbAabb(self, other);
  }

  // if (colliderA.m_type == AABB && colliderB.m_type == PLANE) {
  //   auto &self = static_cast<const Aabb &>(colliderA);
  //   auto &other = static_cast<const Plane &>(colliderB);
  //   return IntersectAabbPlane(self, other);
  // }

  if (colliderA.m_type == PLANE && colliderB.m_type == SPHERE) {
    auto &self = static_cast<const Plane &>(colliderA);
    auto &other = static_cast<const BoundingSphere &>(colliderB);
    return IntersectPlaneSphere(self, other);
  }

  // if (colliderA.m_type == PLANE && colliderB.m_type == AABB) {
  //   auto &self = static_cast<const Plane &>(colliderA);
  //   auto &other = static_cast<const Aabb &>(colliderB);
  //   return IntersectPlaneAabb(self, other);
  // }

  // if (colliderA.m_type == PLANE && colliderB.m_type == PLANE) {
  //   auto &self = static_cast<const Plane &>(colliderA);
  //   auto &other = static_cast<const Plane &>(colliderB);
  //   return IntersectPlanePlane(self, other);
  // }

  std::cerr << "Collision between colliders of type: " << colliderA.m_type
            << " and type: " << colliderB.m_type << " is not defined";

  std::exit(-1);
}
} // namespace Physics
