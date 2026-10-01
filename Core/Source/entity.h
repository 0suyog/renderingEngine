#pragma once
#include "Components/CCamera.h"
#include <memory>
#include <typeindex>
#include "Components/CLight.h"
#include "Components/CPhysicsObject.h"
#include "Components/CCollider.h"
#include "Components/CShape.h"
#include "Components/CStatic.h"
#include "Components/CShape.h"
#include "Components/CTransform.h"
#include "Physics/collider.h"
#include "Components/CSpeed.h"
#include "transform.h"
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
  std::shared_ptr<Component::CSpeed> cSpeed;
  std::shared_ptr<Component::CCamera> cCamera;
  std::shared_ptr<Component::CLight> cLight;

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

  Entity &addCSpeed(const float walkspeed = 2, const float sprintSpeed = 4) {
    cSpeed = std::make_shared<Component::CSpeed>(walkspeed, sprintSpeed);
    return *this;
  }

  Entity &addCCamera(std::shared_ptr<Renderer::Camera> &cam,
                     float followDistance) {
    cCamera = std::make_shared<Component::CCamera>(cam, followDistance);
    return *this;
  }

  Entity &addCLight(Renderer::Light light) {
    cLight = std::make_shared<Component::CLight>(light);
    return *this;
  }
  template <typename T, typename... Args> Entity &addComponent(Args &&...args) {
    m_components[typeid(T)] = std::make_shared<T>(std::forward<Args>(args)...);
    return *this;
  }

  template <typename T> std::shared_ptr<T> GetComponent() {
    if (m_components.contains(typeid(T))) {
      return static_cast<std::shared_ptr<T>>(m_components[typeid(T)]);
    }
  }

  template <typename... Components> bool HasComponents() {
    return (HasComponent<Components>() && ...);
  }

private:
  template <typename Component> bool HasComponent() {
    if (m_components.contains(typeid(Component)))
      return true;
    return false;
  }
  Entity(unsigned int id, const std::string &tag) : m_id(id), m_tag(tag) {}
  std::unordered_map<std::type_index, std::shared_ptr<void>> m_components;
  ;
};
} // namespace Core
