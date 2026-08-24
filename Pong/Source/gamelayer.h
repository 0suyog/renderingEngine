#pragma once
#include "Renderer/camera.h"
#include "Renderer/model.h"
#include "Renderer/shader.h"
#include "transform.h"
#include "windowevents.h"
#include "application.h"
#include "event.h"
#include "layer.h"
#include "inputevents.h"
#include <GLFW/glfw3.h>
#include <iostream>
class GameLayer : public Core::Layer {
public:
  GameLayer() {
    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(),
                     GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  void OnUpdate(float ts) override {}

  void OnRender() override {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, cam.getWidth(), cam.getHeight());
    basicShader.use();
    auto modelTransform = Core::Transform();

    modelTransform.scale(glm::vec3(5, 5, 0.5));
    // modelTransform.rotate(glfwGetTime(), glm::vec3(0, 0, -1));
    // modelTransform.revolve(glm::vec3(0), 20.0, 63 * glfwGetTime(),
    //                        glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0, 0.0,
    //                        0.0));

    modelTransform.moveto({0, 0, 100});
    auto perspective = cam.prespectiveMat();
    auto view = cam.lookAt();

    basicShader.SetMat4("model", modelTransform.getTransformationMatrix());
    basicShader.SetMat4("view", view);
    basicShader.SetMat4("projection", perspective);
    leftPaddle.render(basicShader);
  }

  void OnEvent(Core::Event &e) override {
    // std::cerr << "[Event Occured]: " << e.ToString() << std::endl;
    Core::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Core::MouseButtonPressedEvent>(
        [this](Core::MouseButtonPressedEvent &e) {
          return handleMousePress(e);
        });
    dispatcher.Dispatch<Core::KeyPressedEvent>(
        [this](Core::KeyPressedEvent &e) { return handleKeyPress(e); });
    dispatcher.Dispatch<Core::WindowResizeEvent>(
        [this](Core::WindowResizeEvent &e) { return handleWindowResize(e); });
    dispatcher.Dispatch<Core::MouseMovedEvent>(
        [this](Core::MouseMovedEvent &e) { return handleMouseMove(e); });
  }

private:
  bool handleMousePress(Core::MouseButtonPressedEvent &e) {
    std::cerr << "button: " << e.GetMouseButton() << "\n";
    return true;
  }

  bool handleKeyPress(Core::KeyPressedEvent &e) {
    if (e.GetKeyCode() == GLFW_KEY_ESCAPE ||
        e.GetKeyCode() == GLFW_KEY_CAPS_LOCK) {
      auto &app = Core::Application::Get();
      app.Stop();
    }

    return true;
  }

  bool handleWindowResize(Core::WindowResizeEvent &e) {
    cam.setWidth(e.GetWidth());
    cam.setHeight(e.GetHeight());
    return false;
    return false;
  }

  bool handleMouseMove(Core::MouseMovedEvent &e) {
    cam.rotate(e.GetdX(), e.GetdY());
    return false;
  }

  Renderer::Model leftPaddle = Renderer::Model::Quad();
  Renderer::ShaderProgram basicShader = Renderer::ShaderProgram(
      "./shaders/basicshader.vert", "./shaders/basicshader.frag");
  Renderer::Camera cam;
};
