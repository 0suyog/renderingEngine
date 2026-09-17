#pragma once
#include "Physics/intersectData.h"
#include <memory>
namespace Physics {
class Collider {
public:
  enum { SPHERE = 1, AABB, PLANE, SIZE };
  Collider(int type) : m_type(type) {}
  virtual std::shared_ptr<Collider> UsingOrigin(glm::vec3 origin) = 0;
  static IntersectData Intersect(const Collider &self, const Collider &other);

private:
  int m_type;
};

} // namespace Physics
