#pragma once
#include "entityManager.h"
class System {
public:
  void virtual update(float ts) = 0;
  virtual ~System() = default;

protected:
};
