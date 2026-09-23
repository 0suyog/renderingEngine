#include "physicsEngine.h"
#include "Physics/intersectData.h"
#include "Physics/physicsObject.h"
#include "glm/ext/vector_float3.hpp"
#include "utils.h"
#include <iostream>
#include <memory>
namespace Physics {
std::shared_ptr<PhysicsObject>
PhysicsEngine::AddObject(const glm::vec3 &position, const glm::vec3 &velocity,
                         bool hasGravity, bool isStatic) {
  std::shared_ptr<PhysicsObject> object(
      new PhysicsObject(position, velocity, hasGravity, isStatic));
  m_objects.push_back(object);
  return object;
}

void PhysicsEngine::Simulate(float timeStep) {
  if (isPaused)
    return;
  for (auto &obj : m_objects) {
    if (!obj->isStatic && obj->hasGravity) {
      obj->SetAcceleration(obj->GetAcceleration() + gravity);
      obj->Integrate(timeStep);
    }
  }
}

void PhysicsEngine::AddCollision(Collision collision) {
  m_collisions.push_back(collision);
}

void PhysicsEngine::ResolveCollisions() {
  if (isPaused)
    return;
  for (auto &collision : m_collisions) {
    if (!collision.intersection.GetDoesIntersect())
      continue;
    IntersectData &intersectData = collision.intersection;
    auto &objectA = collision.objectA;
    auto &objectB = collision.objectB;
    glm::vec3 toMove = intersectData.GetNormal() * intersectData.GetDistance();
    for (int i = 0; i < 3; i++) {
      toMove[i] = Core::nearZero(toMove[i]) ? 0.0f : toMove[i];
    }
    // std::cerr << "normal: " << intersectData.GetNormal().x << " "
    //           << intersectData.GetNormal().y << " "
    //           << intersectData.GetNormal().z << "\n";
    // if object A isnt static move it by half the penetration backward the
    // normal
    if (!objectA->isStatic && !objectB->isStatic) {
      toMove = toMove / 2.0f;
      objectA->m_position -= toMove;
      objectB->m_position += toMove;
    }
    // if object B isnt static move it by half the penetration too but forward
    // the normal
    else if (!objectA->isStatic && objectB->isStatic) {
      objectA->m_position -= toMove;
    }
    // if object A isn't static but B turns out to be static then again move A
    // by half
    else if (objectA->isStatic && !objectB->isStatic) {
      objectB->m_position += toMove;
    }
    objectA->m_velocity *= 0.99;
    objectB->m_velocity *= 0.99;
  }
  // cloar all collisions after resolving them
  m_collisions.clear();
}

} // namespace Physics
