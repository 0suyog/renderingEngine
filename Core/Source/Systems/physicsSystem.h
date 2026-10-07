#pragma once
#include "Physics/physicsObject.h"
#include "Physics/physicsEngine.h"
#include "Systems/system.h"
#include "entity.h"
class SPhysicsSystem : public System {
public:
  void detectCollision(std::shared_ptr<Core::Entity> e1,
                       std::shared_ptr<Core::Entity> e2);
  void update(std::shared_ptr<Core::Entity>, float ts) override;
  void resolveCollisions();
  void sync(std::shared_ptr<Core::Entity> e);
  void setGravity(float gravity) { physicsEngine.gravity = gravity; }

private:
  Physics::PhysicsEngine physicsEngine = Physics::PhysicsEngine();
};
