#pragma once
#include "Physics/collider.h"
#include <memory>
namespace Component {
struct CCollider {
  std::shared_ptr<Physics::Collider> collider;
};
} // namespace Component
