#include <memory>
#include <vector>
#include "Physics/collision.h"
#include "physicsObject.h"
namespace Physics {
class PhysicsEngine {
public:
  PhysicsEngine() {};

  std::shared_ptr<PhysicsObject> AddObject(const glm::vec3 &position,
                                           const glm::vec3 &velocity,
                                           bool hasGravity = true,
                                           bool isStatic = false);
  void AddCollision(Collision collision);
  void Simulate(float timeStep);

  void ResolveCollisions();

public:
  glm::vec3 gravity = {0, -20, 0};

private:
  std::vector<std::shared_ptr<PhysicsObject>> m_objects;
  std::vector<Collision> m_collisions;
};
} // namespace Physics
