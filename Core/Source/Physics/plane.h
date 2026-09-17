#pragma once
#include "Physics/collider.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
namespace Physics {
class Plane : public Collider {
public:
  Plane(const glm::vec3 &normal, float distance)
      : Collider(Collider::PLANE), m_normal(normal), m_distance(distance) {}
  Plane(const glm::vec3 &normal, const glm::vec3 &coordInPlane)
      : Collider(Collider::PLANE), m_normal(normal),
        m_distance(glm::dot(normal, coordInPlane)) {}

  const glm::vec3 &getNormal() const { return m_normal; }
  float getDistance() const { return m_distance; }

  std::shared_ptr<Collider> UsingOrigin(glm::vec3 origin) override {
    return std::make_shared<Plane>(m_normal,
                                   m_distance + glm::dot(m_normal, origin));
  }

  Plane Normalized() const {
    auto normalLength = glm::length(m_normal);
    return Plane(m_normal / normalLength, m_distance / normalLength);
  }

private:
  const glm::vec3 m_normal;
  const float m_distance;
};
} // namespace Physics
