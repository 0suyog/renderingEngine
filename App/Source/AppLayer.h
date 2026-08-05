#pragma once
#include "2dprimitives.h"
#include "camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "texture.h"
#include "transform.h"
#include "windowevents.h"
#include "movement.h"
#include "shader.h"
#include "application.h"
#include "event.h"
#include "layer.h"
#include "inputevents.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <memory>
class AppLayer : public Core::Layer {
public:
  AppLayer() {

    Renderer::Triangle(glm::vec3(0, 1, 2), glm::vec3(3, 4, 5),
                       glm::vec3(6, 7, 8));

    tex = Renderer::TextureFromFile("./images/astronaut.jpg", "diffuse");
    tex = Renderer::TextureFromFile("./images/astronaut.jpg", "diffuse");
    tex = Renderer::TextureFromFile("./images/astronaut.jpg", "diffuse");

    if (tex == nullptr) {
      std::cerr << "Failed creating Texture" << "\n";
      exit(-1);
    }

    float vertices[] = {
        // position            // uv
        -5.0f, -5.0f, -10.0f, 0.0, 0.0, // A
        5.0f,  -5.0f, -10.0f, 1.0, 0.0, // B
        5.0f,  5.0f,  -10.0f, 1.0, 1.0, // C
        -5.0f, 5.0f,  -10.0f, 0.0, 1.0  // D
    };

    uint indices[] = {0, 1, 2, 0, 2, 3};

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);
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
    glClearColor(1.0f, 0.0f, 0.0f, 0.001f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, cam.getWidth(), cam.getHeight());
    basicshader.use();
    auto modelTransform = Core::Transform();
    modelTransform.scale(glm::vec3(2 * (sin(glfwGetTime()) + 1.1),
                                   // 2 * (sin(glfwGetTime()) + 1.1),
                                   1.0, 1.0));
    modelTransform.rotate(glfwGetTime(), glm::vec3(0, 0, -1));
    modelTransform.revolve(glm::vec3(0), 20.0, 63 * glfwGetTime(),
                           glm::vec3(0.0, 0.0, 1.0), glm::vec3(1.0, 0.0, 0.0));

    auto perspective = cam.prespectiveMat();
    auto view = cam.lookAt();

    glBindTextureUnit(0, tex->handle);

    basicshader.SetMat4("model", modelTransform.getTransformationMatrix());
    basicshader.SetMat4("view", view);
    basicshader.SetMat4("projection", perspective);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
  }

private:
  bool handleMousePress(Core::MouseButtonPressedEvent &e) {
    std::cerr << "button: " << e.GetMouseButton() << "\n";
    return true;
  }

  bool handleKeyPress(Core::KeyPressedEvent &e) {
    std::cerr << "char: " << e.GetPrintableChar() << '\n';
    if (e.GetKeyCode() == GLFW_KEY_ESCAPE ||
        e.GetKeyCode() == GLFW_KEY_CAPS_LOCK) {
      auto &app = Core::Application::Get();
      app.Stop();
    }

    return true;
  }

  // bool handleWindowClose(Core::WindowClosedEvent &e) { return false; }

  bool handleWindowResize(Core::WindowResizeEvent &e) {
    cam.setWidth(e.GetWidth());
    cam.setHeight(e.GetHeight());
    return false;
  }

  Renderer::ShaderProgram basicshader = {"./shaders/basicshader.vert",
                                         "shaders/basicshader.frag"};
  GLuint vbo, ebo, vao;
  Core::Camera cam;
  std::shared_ptr<Renderer::Texture> tex;
};
