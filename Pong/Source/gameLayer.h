#pragma once
#include "Renderer/camera.h"
#include "Renderer/shader.h"
#include "ball.h"
#include "paddle.h"
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
  int viewportWidth = 960;
  int viewportHeight = 540;
  GameLayer() {
    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(),
                     GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    cam.setWidth(viewportWidth);
    cam.setHeight(viewportHeight);
    cam.fov = 30.0f;
    cam.position({0, 0, 100});
    cam.rotation(0, 0, 0);
  }

  void OnUpdate(float ts) override {
    ball.update(ts);
    ball.collided(leftPaddle);
    ball.collided(rightPaddle);
  }

  void OnRender() override {
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glViewport(0, 0, viewportWidth, viewportHeight);
    // glViewport(0, 0, cam.getWidth(), cam.getHeight());
    basicShader.use();
    auto modelTransform = Core::Transform();

    modelTransform.scale(glm::vec3(50, 50, 0.5));
    // modelTransform.rotate(glfwGetTime(), glm::vec3(0, 0, -1));
    // modelTransform.revolve(glm::vec3(0), 20.0, 63 * glfwGetTime(),
    //                        glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0, 0.0,
    //                        0.0));

    modelTransform.moveto({0, 0, 0});
    auto perspective = cam.prespectiveMat();
    auto view = cam.lookAt();
    basicShader.SetMat4("model", modelTransform.matrix());
    basicShader.SetMat4("view", view);
    basicShader.SetMat4("projection", perspective);
    // leftPaddle.render(basicShader);
    // rightPaddle.render(basicShader);
    ball.render(basicShader);
  }

  void OnEvent(Core::Event &e) override {
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
    auto pos = leftPaddle.transform.matrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1);
    std::cerr << "paddle position normally\n";
    std::cerr << leftPaddle.transform.getPosition().x << " "
              << leftPaddle.transform.getPosition().y << " "
              << leftPaddle.transform.getPosition().z << "\n";
    std::cerr << "paddle position after multiplying\n";
    std::cerr << pos.x << " " << pos.y << " " << pos.z << "\n";
    return true;
  }

  bool handleKeyPress(Core::KeyPressedEvent &e) {
    if (e.GetKeyCode() == GLFW_KEY_ESCAPE ||
        e.GetKeyCode() == GLFW_KEY_CAPS_LOCK) {
      auto &app = Core::Application::Get();
      app.Stop();
    }

    if (e.GetKeyCode() == GLFW_KEY_UP) {
      rightPaddle.moveUp();
    }
    if (e.GetKeyCode() == GLFW_KEY_DOWN) {
      rightPaddle.moveDown();
    }

    // if (e.GetKeyCode() == GLFW_KEY_LEFT) {
    //   leftPaddle.moveLeft();
    // }
    // if (e.GetKeyCode() == GLFW_KEY_RIGHT) {
    //   leftPaddle.moveRight();
    // }

    return true;
  }

  bool handleWindowResize(Core::WindowResizeEvent &e) {
    glViewport(e.GetWidth() / 2 - (viewportWidth / 2),
               e.GetHeight() / 2 - (viewportHeight / 2), viewportWidth,
               viewportHeight);
    // glViewport(0, -500, viewportWidth, viewportHeight);
    // cam.setWidth(e.GetWidth());
    // cam.setHeight(e.GetHeight());
    return false;
  }

  bool handleMouseMove(Core::MouseMovedEvent &e) {
    // if (e.GetdX() != 0 || e.GetdY() != 0) {
    //   cam.rotateByMouse(e.GetdX(), e.GetdY());
    // }
    return false;
  }

  Paddle leftPaddle = Paddle({-46, 0, 0});
  Paddle rightPaddle = Paddle({46, 0, 0});
  Ball ball = Ball({0, 0, 0});

  Renderer::ShaderProgram basicShader = Renderer::ShaderProgram(
      "./shaders/basicshader.vert", "./shaders/basicshader.frag");
  Renderer::Camera cam;
};
