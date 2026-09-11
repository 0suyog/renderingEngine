#include "physicsObject.h"
namespace Physics {
void PhysicsObject::Integrate(float timeStep) {
  m_position += m_velocity * timeStep;
}
} // namespace Physics
