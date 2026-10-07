#pragma once
#include "Components/CCamera.h"
#include <iostream>
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

  template <typename T, typename... Args> Entity &addComponent(Args &&...args) {
    m_components[typeid(T)] = std::make_shared<T>(std::forward<Args>(args)...);
    return *this;
  }

  template <typename T> std::shared_ptr<T> GetComponent() {
    if (m_components.contains(typeid(T))) {
      return std::static_pointer_cast<T>(m_components[typeid(T)]);
    }
    return nullptr;
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
