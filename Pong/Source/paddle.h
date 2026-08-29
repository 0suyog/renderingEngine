#pragma once
#include "Renderer/model.h"
#include "Renderer/renderables.h"
#include "Renderer/shader.h"
#include "transform.h"
#include <iostream>
class Paddle : public Renderer::Renderable {
public:
  Paddle(glm::vec3 position) { transform.moveto(position); }
  float width = 3.0f, height = 15.0f;
  float speed = 1.0f;
  Renderer::Model quad = Renderer::Model::Quad(width, height);
  Core::Transform transform;
  bool isMaxHeight = false, isMinHeight = false;

  glm::vec3 topleft() {
    return transform.getPosition() + glm::vec3(-width / 2, height / 2, 0);
  }
  glm::vec3 topright() {
    return transform.getPosition() + glm::vec3(width / 2, height / 2, 0);
  }
  glm::vec3 bottomLeft() {
    return transform.getPosition() + glm::vec3(-width / 2, -height / 2, 0);
  }
  glm::vec3 bottomRight() {
    return transform.getPosition() + glm::vec3(width / 2, -height / 2, 0);
  }

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
