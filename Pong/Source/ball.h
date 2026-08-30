#include "Renderer/model.h"
#include "Renderer/renderables.h"
#include "Renderer/shader.h"
#include "glm/ext/vector_float3.hpp"
#include "paddle.h"
#include "shapes.h"
#include "transform.h"
#include <cstdlib>
class Ball : public Renderer::Renderable {
public:
  Core::Transform transform;
  glm::vec3 direction = {0.7, 0.3, 0};
  float speed = 50.0;
  float radius = 10;
  Shape::Polygon quad = Shape::Polygon({0, 0, 0}, radius, 6, {1, 1, 1});

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

  bool collided(const Paddle &p) {
    // check if x collided
    bool isLeft = 1;
    if (p.transform.getPosition().x > 0) {
      isLeft = 0;
    }
    if (isLeft) {
      if (transform.getPosition().x - p.topright().x < radius) {
        direction *= -1;
        if (std::rand() < RAND_MAX / 2) {
          direction.y *= -1;
        }
        return true;
      }
    } else {
      if (p.topleft().x - transform.getPosition().x < radius) {
        direction *= -1;
        if (std::rand() < RAND_MAX / 2) {
          direction.y *= -1;
        }
        return true;
      }
    }

    return false;
  }
};
