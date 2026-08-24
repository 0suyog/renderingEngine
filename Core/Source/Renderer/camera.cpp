#include "camera.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "movement.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
namespace Renderer {
glm::mat4 Camera::prespectiveMat() const {
  // TODO: make this by myself without relying on glm
  return glm::perspective(glm::radians(fov), v_Width / v_Height, near, far);
}

glm::mat4 Camera::orthographicMat() const {
  return glm::ortho(-v_Width / 2.0f, v_Width / 2.0f, -v_Height / 2.0f,
                    v_Height / 2.0f, near, far);
}

Camera &Camera::rotation(float pitch, float yaw, float roll) {
  transform.rotate(pitch, yaw, roll);
  forward =
      glm::normalize(glm::vec3(cos(glm::radians(transform.getYaw())) *
                                   cos(glm::radians(transform.getPitch())),
                               sin(glm::radians(transform.getPitch())),
                               sin(glm::radians(transform.getYaw())) *
                                   cos(glm::radians(transform.getPitch()))));
  right = glm::normalize(glm::cross(forward, {0, 1, 0}));
  up = glm ::cross(right, forward);
  std::cerr << "pitch: " << transform.getPitch()
            << "yaw: " << transform.getYaw() << "roll: " << transform.getRoll()
            << "\n";
  return *this;
}

Camera &Camera::position(const glm::vec3 &pos) {
  transform.moveto(pos);
  return *this;
}

glm::mat4 Camera::lookAt() const {
  // TODO: make this by myself without relying on glm
  return glm::lookAt(transform.getPosition(), transform.getPosition() + forward,
                     up);
}

void Camera::rotateByMouse(float dx, float dy) {
  transform.rotate(transform.getPitch() + dy * sensitivity,
                   transform.getYaw() + dx * sensitivity, 0);
  std::cerr << "pitch: " << transform.getPitch()
            << "yaw: " << transform.getYaw() << "roll: " << transform.getRoll()
            << "\n";
  forward =
      glm::normalize(glm::vec3(cos(glm::radians(transform.getYaw())) *
                                   cos(glm::radians(transform.getPitch())),
                               sin(glm::radians(transform.getPitch())),
                               sin(glm::radians(transform.getYaw())) *
                                   cos(glm::radians(transform.getPitch()))));
  right = glm::normalize(glm::cross(forward, {0, 1, 0}));
  up = glm ::cross(right, forward);
}

void Camera::move(Core::Movement movement, float timeStep) {
  glm::vec3 movementVector(0.0f);
  // std::cerr << "movement: " << movementVector.x << " " << movementVector.y
  //           << " " << movementVector.z << "\n";
  float speed = walkSpeed;
  if (movement & Core::LEFT) {
    movementVector -= right;
  }
  if (movement & Core::RIGHT) {
    movementVector += right;
  }
  if (movement & Core::UP) {
    movementVector += up;
  }
  if (movement & Core::DOWN) {
    movementVector -= up;
  }
  if (movement & Core::FORWARD) {
    movementVector += forward;
  }
  if (movement & Core::BACKWARD) {
    movementVector -= forward;
  }
  if (movement & Core::FAST) {
    speed = sprintSpeed;
  }

  movementVector = glm::normalize(movementVector);

  if (glm::isnan(movementVector).y) {
    return;
  }

  // std::cerr << "movement: " << movementVector.x << " " << movementVector.y
  //           << " " << movementVector.z << "\n";

  transform.moveto(transform.getPosition() + speed * movementVector * timeStep);
}

} // namespace Renderer
