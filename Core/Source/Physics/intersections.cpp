#include "Physics/aabb.h"
#include "Physics/boundingSphere.h"
#include "Physics/intersectData.h"
#include "Physics/plane.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/vector_relational.hpp"
#include <linux/limits.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include <iostream>
namespace Physics {

IntersectData Reverse(const IntersectData &id) {
  return IntersectData(id.GetDoesIntersect(), id.GetDistance(),
                       -1.0f * id.GetNormal());
}

// aabb aabb
IntersectData IntersectAabbAabb(const Aabb &aabb1, const Aabb &aabb2) {
  auto distance1 = aabb2.GetMinExtents() - aabb1.GetMaxExtents();
  auto distance2 = aabb1.GetMinExtents() - aabb2.GetMaxExtents();
  auto maxDistances = glm::max(distance1, distance2);
  glm::vec3 normal = {1, 0, 0};

  auto maxDistance = maxDistances.x;
  if (maxDistances.y > maxDistance) {
    normal = {0, 1, 0};
    maxDistance = maxDistances.y;
  }
  if (maxDistances.z > maxDistance) {
    normal = {0, 0, 1};
    maxDistance = maxDistances.z;
  }

  return IntersectData(maxDistance < 0, fabs(maxDistance), normal);
}

// aabb sphere
IntersectData IntersectAabbSphere(const Aabb &aabb,
                                  const BoundingSphere &sphere) {
  glm::vec3 centerDiff = aabb.GetCenter() - sphere.GetCenter();
  bool isInside = (glm::all(glm::lessThan(centerDiff, aabb.GetHalfSize())));

  if (isInside) {
    auto distance = aabb.GetHalfSize() - glm::abs(centerDiff);
    glm::vec3 normal = {1, 0, 0};
    auto signs = glm::sign(glm::vec3(centerDiff));
    float minDist = distance.x;
    if (distance.y < minDist) {
      minDist = distance.y;
      normal = {0, 1, 0};
    }
    if (distance.z < minDist) {
      minDist = distance.z;
      normal = {0, 0, 1};
    }
    normal *= signs;

    return IntersectData(true, minDist, normal);
  } else {
    auto nearestPoint = glm::clamp(sphere.GetCenter(), aabb.GetMinExtents(),
                                   aabb.GetMaxExtents());
    auto difference = sphere.GetCenter() - nearestPoint;
    auto normal = glm::normalize(difference);
    auto distance = glm::length(difference);
    return IntersectData(distance < sphere.Getradius(),
                         fabs(distance - sphere.Getradius()),
                         glm::normalize(difference));
  }
}
IntersectData IntersectSphereAabb(const BoundingSphere &sphere,
                                  const Aabb &aabb) {
  return Reverse(IntersectAabbSphere(aabb, sphere));
}
// aabb plane
IntersectData IntersectAabbPlane(const Aabb &aabb, const Plane &plane) {
  float distanceOfCenterAlongNormal =
      glm::dot(plane.getNormal(), aabb.GetCenter());
  float RadiusAlongPlaneNormal =
      fabs(glm::dot(aabb.GetHalfSize(), plane.getNormal()));
  float maxExtent = distanceOfCenterAlongNormal + RadiusAlongPlaneNormal;
  float minExtent = distanceOfCenterAlongNormal - RadiusAlongPlaneNormal;
  return IntersectData(plane.getDistance() <= maxExtent &&
                           plane.getDistance() >= minExtent,
                       fmin(fabs(plane.getDistance() - minExtent),
                            fabs(plane.getDistance() - maxExtent)),
                       plane.getNormal());

  // auto distance = glm::dot(closestPoint,plane.getNormal());
}
IntersectData IntersectPlaneAabb(const Plane &plane, const Aabb &aabb) {
  return Reverse(IntersectAabbPlane(aabb, plane));
}

// sphere sphere
IntersectData IntersectSphereSphere(const BoundingSphere &sphere1,
                                    const BoundingSphere &sphere2) {
  float radiusDistance = sphere1.Getradius() + sphere2.Getradius();
  auto centerDiff = sphere2.GetCenter() - sphere1.GetCenter();
  float centerDistance = glm::length(centerDiff);
  return IntersectData(centerDistance < radiusDistance,
                       fabs(centerDistance - radiusDistance),
                       glm::normalize(centerDiff));
}

// sphere plane
IntersectData IntersectSpherePlane(const BoundingSphere &sphere,
                                   const Plane &plane) {
  auto normal = glm::normalize(plane.getNormal());
  float distanceToSphereCenter =
      fabs(glm::dot(normal, sphere.GetCenter()) - plane.getDistance());
  float distanceToSphere = distanceToSphereCenter - sphere.Getradius();
  return IntersectData(distanceToSphere < 0, fabs(distanceToSphere),
                       -glm::normalize(plane.getNormal()));
}
IntersectData IntersectPlaneSphere(const Plane &plane,
                                   const BoundingSphere &sphere) {
  return Reverse(IntersectSpherePlane(sphere, plane));
}

// plane plane
IntersectData IntersectPlanePlane(const Plane &plane1, const Plane &plane2);
} // namespace Physics
