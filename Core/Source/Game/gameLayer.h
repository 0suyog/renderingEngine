#pragma once
#include "Components/CLight.h"
#include "Components/CModel.h"
#include "Components/CShape.h"
#include "Components/CTransform.h"
#include "GLFW/glfw3.h"
#include "action.h"
#include "scene.h"
#include "event.h"
#include "inputevents.h"
#include "layer.h"
#include "Renderer/renderer.h"
#include "windowevents.h"
#include <GL/gl.h>
#include <iostream>
#include <memory>
namespace Game {
class GameLayer : public Core::Layer {
public:
  GameLayer() : Layer() {
    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(),
                     GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  void setCurrentScene(std::shared_ptr<Scene> scene) {
    currentScene = scene;
    scene->renderContext.cam->setHeight(windowHeight);
    scene->renderContext.cam->setWidth(windowWidth);
    scene->renderContext.viewPortHeight = windowHeight;
    scene->renderContext.viewPortWidth = windowWidth;
  }

private:
  std::shared_ptr<Scene> currentScene;
  unsigned int windowWidth, windowHeight;
  void OnRender() override {
    // glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_MULTISAMPLE);
    if (currentScene) {
      for (const auto &entity : currentScene->entityManager.GetEntities()) {
        if (entity->HasComponents<Component::CTransform, Component::CShape>()) {
          auto modelMatrix =
              entity->GetComponent<Component::CTransform>()->transform.matrix();
          Renderer::render(currentScene->renderContext,
                           entity->GetComponent<Component::CShape>()->shape,
                           modelMatrix);
        }
        if (entity->HasComponents<Component::CTransform, Component::CModel>()) {
          auto modelMatrix =
              entity->GetComponent<Component::CTransform>()->transform.matrix();
          Renderer::render(currentScene->renderContext,
                           entity->GetComponent<Component::CModel>()->model,
                           modelMatrix);
        }
      }
    }
  }

  void OnUpdate(float ts) override { currentScene->update(ts); }

  void OnEvent(Core::Event &e) override {
    Core::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Core::KeyPressedEvent>(
        [this](Core::KeyPressedEvent &e) { return handleKeyPress(e); });
    dispatcher.Dispatch<Core::KeyReleasedEvent>(
        [this](Core::KeyReleasedEvent &e) { return handleKeyRelease(e); });
    dispatcher.Dispatch<Core::WindowResizeEvent>(
        [this](Core::WindowResizeEvent &e) { return handleWindowResize(e); });
    dispatcher.Dispatch<Core::MouseMovedEvent>(
        [this](Core::MouseMovedEvent &e) { return handleMouseMove(e); });
  }

  bool handleKeyPress(const Core::KeyPressedEvent &e) {
    if (!currentScene) {
      std::cerr << "[GAMELAYER] no scene is selected, check it" << "\n";
      return false;
    }
    if (currentScene->actionMap.contains(e.GetKeyCode())) {
      Core::Action action{currentScene->actionMap[e.GetKeyCode()], true};
      currentScene->doAction(action);
      return true;
    }
    return false;
  }

  bool handleKeyRelease(const Core::KeyReleasedEvent &e) {
    if (!currentScene) {
      std::cerr << "[GAMELAYER] no scene is selected, check it" << "\n";
      return false;
    }
    if (currentScene->actionMap.contains(e.GetKeyCode())) {
      Core::Action action{currentScene->actionMap[e.GetKeyCode()], false};
      currentScene->doAction(action);
      return true;
    }
    return false;
  }

  bool handleWindowResize(const Core::WindowResizeEvent &e) {
    windowHeight = e.GetHeight();
    windowWidth = e.GetWidth();
    if (currentScene) {
      currentScene->renderContext.cam->setHeight(windowHeight);
      currentScene->renderContext.cam->setWidth(windowWidth);
      currentScene->renderContext.viewPortHeight = windowHeight;
      currentScene->renderContext.viewPortWidth = windowWidth;
    }
    return false;
  }

  bool handleMouseMove(const Core::MouseMovedEvent &e) {
    if (!currentScene)
      return false;
    currentScene->onMouseMove(e.GetdX(), e.GetdY());
    return true;
  }

  bool handleMouseButtonPress(const Core::MouseButtonPressedEvent &e) {
    if (!currentScene && !currentScene->actionMap.contains(e.GetMouseButton()))
      return false;

    Core::Action action{.type = currentScene->actionMap[e.GetMouseButton()],
                        .isStart = true};
    currentScene->doAction(action);
  }

  bool handleMouseButtonRelease(const Core::MouseButtonReleasedEvent &e) {
    if (!currentScene && !currentScene->actionMap.contains(e.GetMouseButton()))
      return false;
    Core::Action action{.type = currentScene->actionMap[e.GetMouseButton()],
                        .isStart = false};

    currentScene->doAction(action);
  }
};
} // namespace Game
