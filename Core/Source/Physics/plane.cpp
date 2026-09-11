#include "Physics/plane.h"
#include "Physics/boundingSphere.h"
#include "Physics/intersectData.h"
#include "glm/geometric.hpp"
#include <iostream>
namespace Physics {
Plane Plane::Normalized() const {
  auto normalLength = glm::length(m_normal);
  return Plane(m_normal / normalLength, m_distance / normalLength);
}

IntersectData
Plane::IntersectBoundingSphere(const BoundingSphere &sphere) const {
  float distanceToSphereCenter =
      fabs(glm::dot(m_normal, sphere.GetCenter()) - m_distance);
  float distanceToSphere = distanceToSphereCenter - sphere.Getradius();
  std::cerr << "moew: " << distanceToSphereCenter << "\n";
  std::cerr << "moew" << distanceToSphere << "\n";
  return IntersectData(distanceToSphere < 0, distanceToSphere);
}

} // namespace Physics
