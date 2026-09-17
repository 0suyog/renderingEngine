#pragma once
#include "Physics/intersectData.h"
#include "Physics/physicsObject.h"
#include <memory>
namespace Physics {
/** normal in intersection data will always point from objectA and objectB*/
struct Collision {
  IntersectData intersection;
  std::shared_ptr<PhysicsObject> objectA;
  std::shared_ptr<PhysicsObject> objectB;
};
} // namespace Physics
