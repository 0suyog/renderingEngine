#include "glm/ext/vector_float3.hpp"
namespace Physics {
class PhysicsObject {
public:
  PhysicsObject(const glm::vec3 &position, const glm::vec3 &velocity)
      : m_position(position), m_velocity(velocity) {}

  const glm::vec3 &GetPosition() const { return m_position; }

  const glm::vec3 &GetVelocity() const { return m_velocity; }

  void SetPosition(const glm::vec3 &position) { m_position = position; }

  void SetVelocity(const glm::vec3 &velocity) { m_velocity = velocity; }

  void Integrate(float timeStep);

private:
  glm::vec3 m_position;
  glm::vec3 m_velocity;
};
} // namespace Physics
