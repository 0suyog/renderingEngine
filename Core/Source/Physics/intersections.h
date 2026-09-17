#include "Physics/aabb.h"
#include "Physics/boundingSphere.h"
#include "Physics/intersectData.h"
#include "Physics/plane.h"
namespace Physics {
// aabb aabb
IntersectData IntersectAabbAabb(const Aabb &aabb1, const Aabb &aabb2);

// aabb sphere
IntersectData IntersectAabbSphere(const Aabb &aabb,
                                  const BoundingSphere &sphere);
IntersectData IntersectSphereAabb(const BoundingSphere &sphere,
                                  const Aabb &aabb);

// aabb plane
IntersectData IntersectAabbPlane(const Aabb &aabb, const Plane &plane);
IntersectData IntersectPlaneAabb(const Plane &plane, const Aabb &aabb);

// sphere sphere
IntersectData IntersectSphereSphere(const BoundingSphere &sphere1,
                                    const BoundingSphere &sphere2);

// sphere plane
IntersectData IntersectSpherePlane(const BoundingSphere &sphere,
                                   const Plane &plane);
IntersectData IntersectPlaneSphere(const Plane &plane,
                                   const BoundingSphere &sphere);

// plane plane
IntersectData IntersectPlanePlane(const Plane &plane1, const Plane &plane2);
} // namespace Physics
