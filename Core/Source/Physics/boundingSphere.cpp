#include "Physics/boundingSphere.h"
#include "Physics/intersectData.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
// #include <iostream>
namespace Physics {
IntersectData
BoundingSphere::IntersectBoundingSphere(const BoundingSphere &other) {
  float radiusDistance = m_radius + other.Getradius();
  float centerDistance = glm::length(other.GetCenter() - m_center);
  // std::cerr << "meow "
  //           << glm::length(other.GetCenter() - m_center) - radiusDistance
  //           << "\n";
  // std::cerr << "meow " << glm::to_string(other.GetCenter()) << "\n";
  // std::cerr << "meow " << glm::to_string(m_center) << "\n";
  return IntersectData(centerDistance < radiusDistance,
                       centerDistance - radiusDistance);
}
} // namespace Physics
