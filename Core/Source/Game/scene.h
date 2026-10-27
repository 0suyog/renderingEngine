#pragma once
#include "../action.h"
#include "../entity.h"
#include "../entityManager.h"
#include "../Renderer/text.h"
#include "../Physics/physicsEngine.h"
#include "../Renderer/renderContext.h"
#include "../Systems/physicsSystem.h"
#include <bitset>
namespace Game {
class GameLayer;
class Scene {
public:
  std::unordered_map<unsigned int, unsigned int> actionMap;
  std::shared_ptr<Core::Entity> player;
  SPhysicsSystem sPhysics = SPhysicsSystem();
  Physics::PhysicsEngine physicsEngine;
  Core::EntityManager entityManager;
  Renderer::RenderContext renderContext;
  Renderer::TextManager textManager;
  std::bitset<100> activeActions;
  glm::vec3 worldUp = {0, 1, 0};

  virtual void update(float ts) {};
  virtual void doAction(const Core::Action &action) {};

  virtual void onMouseMove(float dx, float dy) {};

  /**
   * @brief Takes either mouseButton or Keypress like GLFW_MOUSE_BUTTON_1 or
   * GLFW_KEY_W and Core::Action::Forward as param
   *
   * @param input glfw mousebutton or key press
   * @param action What action to perform with the particular input
   */
  // TODO: Make this check for collision on input maps
  void registerAction(unsigned int input, unsigned int action) {
    actionMap[input] = action;
  }
};
} // namespace Game
