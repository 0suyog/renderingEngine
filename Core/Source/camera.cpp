#include "camera.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "movement.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
namespace Core {
glm::mat4 Camera::prespectiveMat() const {
  // TODO: make this by myself without relying on glm
  return glm::perspective(fov, aspectRatio, near, far);
}

glm::mat4 Camera::lookAt() const {
  // TODO: make this by myself without relying on glm
  return glm::lookAt(position, position + forward, up);
}

void Camera::move(Core::Movement movement, float timeStep) {
  glm::vec3 movementVector(0.0f);
  // std::cerr << "movement: " << movementVector.x << " " << movementVector.y
  //           << " " << movementVector.z << "\n";
  float speed = walkSpeed;
  if (movement & LEFT) {
    movementVector -= right;
  }
  if (movement & RIGHT) {
    movementVector += right;
  }
  if (movement & UP) {
    movementVector += up;
  }
  if (movement & DOWN) {
    movementVector -= up;
  }
  if (movement & FORWARD) {
    movementVector += forward;
  }
  if (movement & BACKWARD) {
    movementVector -= forward;
  }
  if (movement & FAST) {
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

} // namespace Core
