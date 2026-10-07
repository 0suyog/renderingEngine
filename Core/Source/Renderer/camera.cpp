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

  right = glm::normalize(glm::cross(forward, worldUp));
  up = glm::normalize(glm::cross(right, forward));
}

Camera &Camera::setRotation(float pitch, float yaw, float roll) {
  transform.rotate(pitch, yaw, roll);
  updateVectors();
  return *this;
}

Camera &Camera::setPosition(const glm::vec3 &pos) {
  transform.setPosition(pos);
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

void Camera::move(glm::vec3 movementVector, float timeStep, bool isFast) {
  transform.setPosition(transform.getPosition() +
                        (isFast ? sprintSpeed : walkSpeed) * movementVector *
                            timeStep);
}

void Camera::follow(const glm::vec3 &anchor, float distance) {
  auto pos = anchor - (distance * forward);
  setPosition(pos);
}

} // namespace Renderer
