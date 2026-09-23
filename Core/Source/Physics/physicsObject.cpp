#include "physicsObject.h"
#include "glm/ext/vector_float3.hpp"
#include "utils.h"
#include <iostream>
namespace Physics {
void PhysicsObject::Integrate(float timeStep) {
  for (int i = 0; i < 3; i++) {
    m_acceleration[i] =
        Core::nearZero(m_acceleration[i]) ? 0.0f : m_acceleration[i];
  }
  m_velocity += m_acceleration * timeStep;
  for (int i = 0; i < 3; i++) {
    m_velocity[i] = Core::nearZero(m_velocity[i]) ? 0.0f : m_velocity[i];
  }
  m_position += m_velocity * timeStep;
  for (int i = 0; i < 3; i++) {
    m_position[i] = Core::nearZero(m_position[i]) ? 0.0f : m_position[i];
  }

  // std::cerr << "m_velocity: " << m_velocity.x << " " << m_velocity.y << " "
  //           << m_velocity.z << "\n";
  // std::cerr << "m_acceleration: " << m_acceleration.x << " " <<
  // m_acceleration.y
  //           << " " << m_acceleration.z << "\n";
  // std::cerr << "m_position: " << m_position.x << " " << m_position.y << " "
  //           << m_position.z << "\n";
  m_acceleration = glm::vec3(0);
}

void PhysicsObject::applyForce(const glm::vec3 &force) {
  m_acceleration += force / mass;
  std::cerr << "meow\n";
}

} // namespace Physics
