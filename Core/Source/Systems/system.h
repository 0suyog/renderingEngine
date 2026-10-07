#pragma once
#include "entity.h"
#include <memory>
class System {
public:
  void virtual update(std::shared_ptr<Core::Entity>, float ts) = 0;
  virtual ~System() = default;

protected:
};
