#pragma once
#include "Physics/collider.h"
#include "glm/ext/vector_float3.hpp"
#include <memory>
namespace Physics {
class Aabb : public Collider {
public:
  Aabb(glm::vec3 minExtents, glm::vec3 maxExtents)
      : Collider(Collider::AABB), m_minExtents(minExtents),
        m_maxExtents(maxExtents), size(maxExtents - minExtents),
        halfSize(size / 2.0f), center(maxExtents - halfSize) {}

  const glm::vec3 &GetMinExtents() const { return m_minExtents; }
  const glm::vec3 &GetMaxExtents() const { return m_maxExtents; }

  const glm::vec3 &GetSize() const { return size; }
  const glm::vec3 &GetHalfSize() const { return halfSize; }
  const glm::vec3 &GetCenter() const { return center; }

  std::shared_ptr<Collider> UsingOrigin(glm::vec3 origin) override {
    return std::make_shared<Aabb>(m_minExtents + origin, m_maxExtents + origin);
  }

private:
  const glm::vec3 m_minExtents;
  const glm::vec3 m_maxExtents;
  const glm::vec3 size;
  const glm::vec3 halfSize;
  const glm::vec3 center;
};
} // namespace Physics
