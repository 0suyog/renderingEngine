#include "Renderer/model.h"
#include "Renderer/renderables.h"
#include "Renderer/shader.h"
#include "glm/ext/vector_float3.hpp"
#include "paddle.h"
#include "transform.h"
class Ball : public Renderer::Renderable {
public:
  Core::Transform transform;
  glm::vec3 direction = {1, 0, 0};
  float speed = 2.0;
  float width = 2, height = 2;
  Renderer::Model quad = Renderer::Model::Quad(2, 2);

public:
  Ball(glm::vec3 pos) { transform.moveto(pos); }
  void render(const Renderer::ShaderProgram &s) const override {
    s.use();
    s.SetMat4("model", transform.matrix());
    quad.render(s);
  }
  void update(const float ts) override {
    transform.moveto(transform.getPosition() + (ts * direction * speed));
  }

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

  bool collided(const Paddle &p) {
    // check if x collided
    return false;
  }
};
