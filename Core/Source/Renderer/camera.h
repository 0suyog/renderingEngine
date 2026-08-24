#pragma once
#include "movement.h"
#include "transform.h"
#include <glm/ext/matrix_float4x4.hpp>
namespace Renderer {
class Camera {
public:
  Camera() { rotateByMouse(0, 0); }
  glm::mat4 lookAt() const;
  glm::mat4 prespectiveMat() const;
  glm::mat4 orthographicMat() const;
  void move(Core::Movement directions, float timeStep);

  Camera &position(const glm::vec3 &pos);
  Camera &rotation(float pitch, float yaw, float roll);

  void setWidth(float w) {
    v_Width = w;
    aspectRatio = v_Width / v_Height;
  }

  void setHeight(float h) {
    v_Height = h;
    aspectRatio = v_Width / v_Height;
  }

  float getWidth() const { return v_Width; }
  float getHeight() const { return v_Height; }

  void rotateByMouse(float dx, float dy);
  float fov = 45.0f;
  float sensitivity = 0.05;
  float walkSpeed = 5.0f;
  float sprintSpeed = 9.0f;

private:
  Core::Transform transform;
  float near = 0.01, far = 1000.0f;
  float v_Width = 800, v_Height = 600;
  float aspectRatio = v_Width / v_Height;
  glm::vec3 up = {0, 1, 0}, forward = {0, 0, 1}, right = {1, 0, 0};
};
} // namespace Renderer
