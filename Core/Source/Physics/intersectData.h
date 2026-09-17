#pragma once
#include "glm/ext/vector_float3.hpp"
namespace Physics {
class IntersectData {
public:
  IntersectData(const bool doesIntersect, const float distance,
                const glm::vec3 normal)
      : m_doesIntersect(doesIntersect), m_distance(distance), m_normal(normal) {
  }
  bool GetDoesIntersect() const { return m_doesIntersect; }
  float GetDistance() const { return m_distance; }
  const glm::vec3 &GetNormal() const { return m_normal; }

private:
  bool m_doesIntersect;
  float m_distance;
  glm::vec3 m_normal;
};
} // namespace Physics
