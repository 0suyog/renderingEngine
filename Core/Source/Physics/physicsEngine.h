#include <memory>
#include <vector>
#include "Physics/collision.h"
namespace Physics {
class PhysicsEngine {
public:
  PhysicsEngine() {};

  std::shared_ptr<PhysicsObject>
  AddObject(const glm::vec3 &position, const glm::vec3 &velocity = {0, 0, 0},
            bool hasGravity = true, bool isStatic = false, float mass = 1.0f,
            float friction = 0.0);
  void AddCollision(Collision collision);
  void Simulate(float timeStep);

  void ResolveCollisions();

public:
  glm::vec3 gravity = {0, -0.05, 0};
  float isPaused = false;

private:
  std::vector<std::shared_ptr<PhysicsObject>> m_objects;
  std::vector<Collision> m_collisions;
};
} // namespace Physics
