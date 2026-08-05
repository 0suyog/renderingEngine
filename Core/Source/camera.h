#pragma once
#include "movement.h"
#include <glm/ext/matrix_float4x4.hpp>
namespace Core {
class Camera {
public:
  glm::mat4 lookAt() const;
  glm::mat4 prespectiveMat() const;
  void move(Core::Movement directions, float timeStep);

  void setWidth(float w) { v_Width = w; }

  void setHeight(float h) { v_Height = h; }

  float getWidth() const { return v_Width; }
  float getHeight() const { return v_Height; }

private:
  float fov = 65.0f;
  float yaw = 0, pitch = 0, roll = 0; // in degrees
  float near = 0.01, far = 1000.0f;
  float v_Width = 800, v_Height = 600;
  float aspectRatio = v_Width / v_Height;
  float walkSpeed = 5.0f;
  float sprintSpeed = 9.0f;
  glm::vec3 up = {0, 1, 0}, forward = {0, 0, -1}, right = {1, 0, 0},
            position = {0, 0, 0};
};
} // namespace Core
