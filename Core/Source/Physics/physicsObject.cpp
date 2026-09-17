#include "physicsObject.h"
namespace Physics {
void PhysicsObject::Integrate(float timeStep) {
  m_position += m_velocity * timeStep;
  m_velocity = {0, 0, 0};
}
} // namespace Physics
