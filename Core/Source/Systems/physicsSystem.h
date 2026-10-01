#pragma once
#include "Physics/physicsObject.h"
#include "Systems/system.h"
#include "entityManager.h"
class SPhysicsSystem : public System {
  SPhysicsSystem(Core::EntityManager &entityManager,
                 Physics::PhysicsEngine &physicsEngine)
      : System(entityManager), physicsEngine(physicsEngine) {};

private:
  Physics::PhysicsEngine &physicsEngine;
};
