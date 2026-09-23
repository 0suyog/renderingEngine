#pragma once
#include "action.h"
#include "scene.h"
#include "event.h"
#include "inputevents.h"
#include "layer.h"
#include "Renderer/renderer.h"
namespace Game {
class GameLayer : public Core::Layer {
  std::shared_ptr<Scene> currentScene;

  void OnRender() override {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (currentScene) {
      for (const auto &entity : currentScene->entityManager.GetEntities()) {
        if (entity->cShape && entity->cTransform) {
          auto modelMatrix = entity->cTransform->transform.matrix();
          Renderer::render(currentScene->renderContext, entity->cShape->shape,
                           modelMatrix);
        }
      }
    }
  }

  void OnEvent(Core::Event &e) override {
    Core::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Core::KeyPressedEvent>(
        [this](Core::KeyPressedEvent &e) { return handleKeyPress(e); });
    dispatcher.Dispatch<Core::KeyReleasedEvent>(
        [this](Core::KeyReleasedEvent &e) { return handleKeyRelease(e); });
  }

  bool handleKeyPress(const Core::KeyPressedEvent &e) {
    if (currentScene->actionMap.contains(e.GetKeyCode())) {
      Core::Action action{currentScene->actionMap[e.GetKeyCode()], true};
      currentScene->doAction(action);
      return true;
    }
    return false;
  }
  bool handleKeyRelease(const Core::KeyReleasedEvent &e) {
    if (currentScene->actionMap.contains(e.GetKeyCode())) {
      Core::Action action{currentScene->actionMap[e.GetKeyCode()], false};
      currentScene->doAction(action);
      return true;
    }
    return false;
  }
};
} // namespace Game
