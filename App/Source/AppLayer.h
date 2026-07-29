#pragma once
#include "shader.h"
#include "application.h"
#include "event.h"
#include "layer.h"
#include "inputevents.h"
#include <iostream>
class AppLayer : public Core::Layer {
public:
  AppLayer() {

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // A
        0.5f,  -0.5f, 0.0f, // B
        0.5f,  0.5f,  0.0f, // C
        -0.5f, 0.5f,  0.0f, // D
    };

    uint indices[] = {0, 1, 2, 0, 2, 3};

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);
  }

  void OnRender() override {
    glClearColor(1.0f, 0.0f, 0.0f, 0.001f);
    glClear(GL_COLOR_BUFFER_BIT);
    basicshader.use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

  void OnEvent(Core::Event &e) override {
    std::cerr << "[Event Occured]: " << e.ToString() << std::endl;
    Core::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Core::MouseButtonPressedEvent>(
        [this](Core::MouseButtonPressedEvent &e) {
          return handleMousePress(e);
        });
    dispatcher.Dispatch<Core::KeyPressedEvent>(
        [this](Core::KeyPressedEvent &e) { return handleKeyPress(e); });
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

  Renderer::ShaderProgram basicshader = {"./shaders/basicshader.vert",
                                         "shaders/basicshader.frag"};
  GLuint vbo, ebo, vao;
};
