#pragma once
#include "glm/ext/vector_float3.hpp"
#include <algorithm>
namespace Physics {
class PhysicsEngine;
class PhysicsObject {
public:
  PhysicsObject(const glm::vec3 &position,
                const glm::vec3 &velocity = {0, 0, 0}, bool hasGravity = true,
                bool isStatic = false, float mass = 1.0f, float friction = 0.0f)
      : m_position(position), m_velocity(velocity), hasGravity(hasGravity),
        isStatic(isStatic), mass(mass), friction(std::max(friction, 0.1f)) {}
  const glm::vec3 &GetPosition() const { return m_position; }

  const glm::vec3 &GetVelocity() const { return m_velocity; }

  const glm::vec3 &GetAcceleration() const { return m_acceleration; }

  void SetPosition(const glm::vec3 &position) { m_position = position; }

  void SetVelocity(const glm::vec3 &velocity) { m_velocity = velocity; }

  void SetAcceleration(const glm::vec3 &acc) { m_acceleration = acc; }

  void Integrate(float timeStep);

  const bool hasGravity;
  const bool isStatic;
  friend class PhysicsEngine;
  void applyForce(const glm::vec3 &force);

private:
  glm::vec3 m_position;
  glm::vec3 m_velocity = {0, 0, 0};
  glm::vec3 m_acceleration = {0, 0, 0};
  float mass = 1.0f;
  float friction = 0.0f;
};
} // namespace Physics
