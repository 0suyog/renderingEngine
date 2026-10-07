#pragma once
#include <memory>
#include <vector>
#include "Physics/collision.h"
#include "Physics/physicsObject.h"
namespace Physics {
class PhysicsEngine {
public:
  PhysicsEngine() {};

  void AddCollision(Collision collision);
  void Simulate(std::shared_ptr<PhysicsObject>, float timeStep);

  void ResolveCollisions();

public:
  float gravity = -0.05;
  float isPaused = false;
  glm::vec3 worldUp = {0, 1, 0};

private:
  std::vector<Collision> m_collisions;
};
} // namespace Physics
