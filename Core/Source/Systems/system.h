#pragma once
#include "entityManager.h"
class System {
public:
  explicit System(Core::EntityManager &e) : entitymanager(e) {}
  void virtual update(float ts) = 0;
  virtual ~System() = default;

protected:
  Core::EntityManager &entitymanager;
};
