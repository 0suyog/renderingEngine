#pragma once
#include "Renderer/2dprimitives.h"
#include "Renderer/camera.h"
#include "Renderer/model.h"
#include "Renderer/texture.h"
#include "transform.h"
#include "windowevents.h"
#include "movement.h"
#include "Renderer/shader.h"
#include "application.h"
#include "event.h"
#include "layer.h"
#include "inputevents.h"
#include <GLFW/glfw3.h>
#include <iostream>
class AppLayer : public Core::Layer {
public:
  AppLayer() {
    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(),
                     GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }

  void OnUpdate(float ts) override {
    Core::Movement camMovement = Core::NONE;

    if (Core::KeyEvent::GetKey(GLFW_KEY_W) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::FORWARD);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_S) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::BACKWARD);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_A) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::LEFT);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_D) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::RIGHT);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::DOWN);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::UP);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::FAST);
    }

    cam.move(camMovement, ts);
  }

  void OnRender() override {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, cam.getWidth(), cam.getHeight());
    basicshader.use();
    auto modelTransform = Core::Transform();
    // modelTransform.scale(glm::vec3(0.5, 0.5, 0.5));
    // modelTransform.rotate(glfwGetTime(), glm::vec3(0, 0, -1));
    // modelTransform.revolve(glm::vec3(0), 20.0, 63 * glfwGetTime(),
    //                        glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0, 0.0,
    //                        0.0));
    //
    auto perspective = cam.prespectiveMat();
    auto view = cam.lookAt();

    // glBindTextureUnit(0, tex->handle);

    basicshader.SetMat4("model", modelTransform.getTransformationMatrix());
    basicshader.SetMat4("view", view);
    basicshader.SetMat4("projection", perspective);
    // bag.render(basicshader);
    cube.render(basicshader);
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
    // std::cerr << "char: " << e.GetPrintableChar() << '\n';
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
  }

  bool handleMouseMove(Core::MouseMovedEvent &e) {
    cam.rotate(e.GetdX(), e.GetdY());
    // std::cerr << "dx: " << e.GetdX() << "dy: " << e.GetdY() << "\n";
    return false;
  }

  Renderer::ShaderProgram basicshader = {"./shaders/basicshader.vert",
                                         "shaders/basicshader.frag"};
  Renderer::Model bag = Renderer::Model("./models/chessKing/chessKing.obj");
  Renderer::Model cube = Renderer::Model::Pyramid();
  GLuint vbo, ebo, vao;
  Renderer::Camera cam;
  std::shared_ptr<Renderer::Texture> tex;
};
