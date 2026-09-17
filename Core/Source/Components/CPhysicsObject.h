#pragma once
#include "Physics/physicsObject.h"
#include <memory>
namespace Component {
struct CPhysicsObject {
  std::shared_ptr<Physics::PhysicsObject> physicsObject;
};
} // namespace Component
