#pragma once
#include "Physics/physicsObject.h"
#include "Physics/physicsEngine.h"
#include "Systems/system.h"
#include "entity.h"
class SPhysicsSystem : public System {
  SPhysicsSystem(Physics::PhysicsEngine &physicsEngine)
      : physicsEngine(physicsEngine) {};

  void collision(std::shared_ptr<Core::Entity> e1,
                 std::shared_ptr<Core::Entity> e2);
  void update(float ts) override;
  void sync(std::shared_ptr<Core::Entity> e);

private:
  Physics::PhysicsEngine &physicsEngine;
};
