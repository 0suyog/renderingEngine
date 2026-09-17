#pragma once
#include "Physics/collider.h"
#include "glm/ext/vector_float3.hpp"
#include <memory>
namespace Physics {
class BoundingSphere : public Collider {
public:
  BoundingSphere(glm::vec3 center, float radius)
      : Collider(Collider::SPHERE), m_center(center), m_radius(radius) {}
  float Getradius() const { return m_radius; };
  inline const glm::vec3 &GetCenter() const { return m_center; }

  std::shared_ptr<Collider> UsingOrigin(glm::vec3 origin) override {
    return std::make_shared<BoundingSphere>(m_center + origin, m_radius);
  }

private:
  const glm::vec3 m_center;
  const float m_radius;
};
} // namespace Physics
