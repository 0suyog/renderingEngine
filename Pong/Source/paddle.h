#pragma once
#include "Renderer/model.h"
#include "Renderer/renderables.h"
#include "Renderer/shader.h"
#include "transform.h"
class Paddle : public Renderer::Renderable {
public:
  Paddle(glm::vec3 position) { transform.moveto(position); }

  glm::vec3 velocity = {0.0, 1.0f, 0.0};
  float speed = 0.1f;
  Renderer::Model quad = Renderer::Model::Quad();
  Core::Transform transform;
  bool isMaxHeight = false, isMinHeight = false;

  void render(const Renderer::ShaderProgram &s) const override {
    s.use();
    s.SetMat4("model", transform.matrix());
    quad.render(s);
  }

  void moveUp() {
    if (isMinHeight) {
      isMinHeight = false;
    }
    if (!isMaxHeight) {
      transform.moveto(transform.getPosition() + glm::vec3(0, speed, 0));
    }
  }

  void moveDown() {
    if (isMaxHeight) {
      isMaxHeight = false;
    }
    if (!isMinHeight) {
      transform.moveto(transform.getPosition() - glm::vec3(0, speed, 0));
    }
  }

  void moveLeft() {
    transform.moveto(transform.getPosition() - glm::vec3(speed, 0, 0));
  }
  void moveRight() {
    transform.moveto(transform.getPosition() + glm::vec3(speed, 0, 0));
  }
};
