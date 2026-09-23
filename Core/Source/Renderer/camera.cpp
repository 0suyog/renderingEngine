#include "camera.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "action.h"
#include <glm/ext/matrix_clip_space.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/matrix_transform.hpp>
namespace Renderer {
glm::mat4 Camera::prespectiveMat() const {
  // TODO: make this by myself without relying on glm
  return glm::perspective(glm::radians(fov), aspectRatio, near, far);
}

glm::mat4 Camera::orthographicMat() const {
  // TODO: make this by myself without relying on glm
  return glm::ortho(-v_Width / 2.0f, v_Width / 2.0f, -v_Height / 2.0f,
                    v_Height / 2.0f, near, far);
}

void Camera::updateVectors() {
  float yaw = glm::radians(transform.getYaw());
  float pitch = glm::radians(transform.getPitch());

  forward = glm::normalize(
      glm::vec3(sin(yaw) * cos(pitch), sin(pitch), -cos(yaw) * cos(pitch)));

  right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
  up = glm::normalize(glm::cross(right, forward));
}

Camera &Camera::rotation(float pitch, float yaw, float roll) {
  transform.rotate(pitch, yaw, roll);
  updateVectors();
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
  transform.rotate(transform.getPitch() - dy * sensitivity,
                   transform.getYaw() + dx * sensitivity, 0);
  updateVectors();
}

// void Camera::move(Core::Action movement, float timeStep) {
//   glm::vec3 movementVector(0.0f);
//   float speed = walkSpeed;
//   if (movement & Core::Action::LEFT) {
//     movementVector -= right;
//   }
//   if (movement & Core::Action::RIGHT) {
//     movementVector += right;
//   }
//   if (movement & Core::Action::UP) {
//     movementVector += up;
//   }
//   if (movement & Core::Action::DOWN) {
//     movementVector -= up;
//   }
//   if (movement & Core::Action::FORWARD) {
//     movementVector += forward;
//   }
//   if (movement & Core::Action::BACKWARD) {
//     movementVector -= forward;
//   }
//   if (movement & Core::Action::FAST) {
//     speed = sprintSpeed;
//   }
//
//   movementVector = glm::normalize(movementVector);
//
//   if (glm::isnan(movementVector).y) {
//     return;
//   }
//
//   transform.moveto(transform.getPosition() + speed * movementVector *
//   timeStep);
// }

} // namespace Renderer
