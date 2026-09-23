#pragma once
#include <memory>
#include "Components/CPhysicsObject.h"
#include "Components/CCollider.h"
#include "Components/CShape.h"
#include "Components/CStatic.h"
#include "Components/CShape.h"
#include "Components/CTransform.h"
#include "Physics/collider.h"
#include "Physics/physicsObject.h"
#include "Renderer/shapes3d.h"
#include "transform.h"
#include <string>
namespace Core {

class EntityManager;

class Entity {
public:
  friend EntityManager;

  unsigned int m_id = 0;
  std::string m_tag = "default";
  bool m_alive = true;
  std::shared_ptr<Component::CPhysicsObject> cPhysicsObject;
  std::shared_ptr<Component::CCollider> cCollider;
  std::shared_ptr<Component::CShape> cShape;
  std::shared_ptr<Component::CStatic> cStatic;
  std::shared_ptr<Component::CTransform> cTransform;

  Entity &
  addCPhysicsObject(std::shared_ptr<Physics::PhysicsObject> physicsObject) {
    cPhysicsObject = std::make_shared<Component::CPhysicsObject>(physicsObject);
    return *this;
  }

  Entity &addCCollider(std::shared_ptr<Physics::Collider> collider) {
    cCollider = std::make_shared<Component::CCollider>(collider);
    return *this;
  }

  Entity &addCShape(const Renderer::Shapes3D shape) {
    cShape = std::make_shared<Component::CShape>(shape);
    return *this;
  }

  Entity &addCStatic() {
    cStatic = std::make_shared<Component::CStatic>();
    return *this;
  }

  Entity &addCTransform(const Core::Transform transform) {
    cTransform = std::make_shared<Component::CTransform>(transform);
    return *this;
  }

private:
  Entity(unsigned int id, const std::string &tag) : m_id(id), m_tag(tag) {}
};
} // namespace Core
