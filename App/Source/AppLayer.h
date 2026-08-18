#pragma once
#include "2dprimitives.h"
#include "camera.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "model.h"
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
    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(),
                     GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Renderer::Triangle(glm::vec3(0, 1, 2), glm::vec3(3, 4, 5),
                       glm::vec3(6, 7, 8));

    tex = Renderer::TextureFromFile("./images/barrel.jpg");

    if (tex == nullptr) {
      std::cerr << "Failed creating Texture" << "\n";
      exit(-1);
    }

    float vertices[] = {
        // Front (+Z)
        // position             // normal       // uv
        -5.0f, -5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f, -5.0f, 5.0f,
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 5.0f, 5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, -5.0f, 5.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        // Back (-Z)
        5.0f, -5.0f, -5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -5.0f, -5.0f, -5.0f,
        0.0f, 0.0f, -1.0f, 1.0f, 0.0f, -5.0f, 5.0f, -5.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 5.0f, 5.0f, -5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,

        // Left (-X)
        -5.0f, -5.0f, -5.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -5.0f, -5.0f, 5.0f,
        -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, -5.0f, 5.0f, 5.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -5.0f, 5.0f, -5.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // Right (+X)
        5.0f, -5.0f, 5.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, -5.0f, -5.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 5.0f, 5.0f, -5.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 5.0f, 5.0f, 5.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        // Top (+Y)
        -5.0f, 5.0f, 5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 5.0f, 5.0f, 5.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 5.0f, 5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -5.0f, 5.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        // Bottom (-Y)
        -5.0f, -5.0f, -5.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 5.0f, -5.0f, -5.0f,
        0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 5.0f, -5.0f, 5.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 1.0f, -5.0f, -5.0f, 5.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f};

    uint32_t indices[] = {
        0,  1,  2,  0,  2,  3,  // Front
        4,  5,  6,  4,  6,  7,  // Back
        8,  9,  10, 8,  10, 11, // Left
        12, 13, 14, 12, 14, 15, // Right
        16, 17, 18, 16, 18, 19, // Top
        20, 21, 22, 20, 22, 23  // Bottom
    };
    //
    // float vertices[] = {// position             // normal       // uv
    //                     -5.0f, -5.0f, -10.0f, 0, 0, 1, 0, 0,
    //                     5.0f,  -5.0f, -10.0f, 0, 0, 1, 1, 0,
    //                     5.0f,  5.0f,  -10.0f, 0, 0, 1, 1, 1,
    //                     -5.0f, 5.0f,  -10.0f, 0, 0, 1, 0, 1};
    // //
    // uint indices[] = {0, 1, 2, 0, 2, 3};
    // Not using DSA
    // glGenBuffers(1, &vbo);
    // glGenBuffers(1, &ebo);
    // glGenVertexArrays(1, &vao);
    //
    // glBindVertexArray(vao);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
    // GL_STATIC_DRAW); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 *
    // sizeof(float), 0); glEnableVertexAttribArray(0); glVertexAttribPointer(1,
    // 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
    //                       (void *)(3 * sizeof(float)));
    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
    //              GL_STATIC_DRAW);
    // using DSA
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);
    glCreateVertexArrays(1, &vao);

    // Upload vertex data
    glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Upload index data
    glNamedBufferData(ebo, sizeof(indices), indices, GL_STATIC_DRAW);

    // Tell VAO that binding slot 0 uses vbo
    glVertexArrayVertexBuffer(vao,
                              0, // binding index
                              vbo,
                              0,                // offset
                              8 * sizeof(float) // stride
    );

    // Position: attribute 0
    glEnableVertexArrayAttrib(vao, 0);

    glVertexArrayAttribFormat(vao,
                              0, // attribute index
                              3, // 3 floats
                              GL_FLOAT, GL_FALSE,
                              0 // offset
    );

    glVertexArrayAttribBinding(vao,
                               0, // attribute index
                               0  // binding index
    );
    // normal
    glEnableVertexArrayAttrib(vao, 1);

    glVertexArrayAttribFormat(vao,
                              1, // attribute index
                              3, // 3 floats
                              GL_FLOAT, GL_FALSE,
                              3 * sizeof(float) // offset
    );

    glVertexArrayAttribBinding(vao,
                               1, // attribute index
                               0  // binding index
    );

    // UV: attribute 1
    glEnableVertexArrayAttrib(vao, 2);

    glVertexArrayAttribFormat(vao,
                              2, // attribute index
                              2, // 2 floats
                              GL_FLOAT, GL_FALSE,
                              6 * sizeof(float) // offset
    );

    glVertexArrayAttribBinding(vao,
                               2, // attribute index
                               0  // binding index
    );

    // EBO
    glVertexArrayElementBuffer(vao, ebo);
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

    glBindTextureUnit(0, tex->handle);

    basicshader.SetMat4("model", modelTransform.getTransformationMatrix());
    basicshader.SetMat4("view", view);
    basicshader.SetMat4("projection", perspective);
    // bag.render(basicshader);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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
  // Renderer::Model bag = Renderer::Model("./models/backpack/backpack.obj");
  GLuint vbo, ebo, vao;
  Core::Camera cam;
  std::shared_ptr<Renderer::Texture> tex;
};
