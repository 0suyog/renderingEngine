#pragma once
#include "transform.h"
namespace Component {
class CTransform {
public:
  CTransform(Core::Transform transform) : transform(transform) {}
  Core::Transform transform;
};
} // namespace Component
