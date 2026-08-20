#include "camera.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "movement.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
namespace Renderer {
glm::mat4 Camera::prespectiveMat() const {
  // TODO: make this by myself without relying on glm
  return glm::perspective(glm::radians(fov), v_Width / v_Height, near, far);
}

glm::mat4 Camera::lookAt() const {
  // TODO: make this by myself without relying on glm
  return glm::lookAt(position, position + forward, up);
}

void Camera::rotate(float dx, float dy) {
  pitch += dy * sensitivity;
  yaw += dx * sensitivity;
  forward = -glm::normalize(
      glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                sin(glm::radians(pitch)),
                sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
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

  position += speed * movementVector * timeStep;
}

} // namespace Renderer
