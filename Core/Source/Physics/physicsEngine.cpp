#include "physicsEngine.h"
namespace Physics {
void PhysicsEngine::AddObject(const PhysicsObject &object) {
  m_objects.push_back(object);
}

void PhysicsEngine::Simulate(float timeStep) {
  for (auto &obj : m_objects) {
    obj.Integrate(timeStep);
  }
}

} // namespace Physics
