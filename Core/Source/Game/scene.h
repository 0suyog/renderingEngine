#pragma once
#include "../action.h"
#include "../entity.h"
#include "../entityManager.h"
#include "../Renderer/renderContext.h"
#include "../Physics/physicsEngine.h"
namespace Game {
class Scene {
public:
  std::unordered_map<unsigned int, unsigned int> actionMap;
  std::shared_ptr<Core::Entity> player;
  Physics::PhysicsEngine physicsEngine;
  Core::EntityManager entityManager;
  Renderer::RenderContext renderContext;

  virtual void update(float ts) = 0;
  virtual void doAction(Core::Action) = 0;
};
} // namespace Game
