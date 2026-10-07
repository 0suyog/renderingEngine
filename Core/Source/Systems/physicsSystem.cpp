#include "physicsSystem.h"
#include "Components/CPhysicsObject.h"

void SPhysicsSystem::detectCollision(std::shared_ptr<Core::Entity> e1,
                                     std::shared_ptr<Core::Entity> e2) {

  auto objectA = e1->GetComponent<Component::CPhysicsObject>()->physicsObject;

  auto colliderA =
      e1->GetComponent<Component::CCollider>()->collider->UsingOrigin(
          objectA->GetPosition());

  auto objectB = e2->GetComponent<Component::CPhysicsObject>()->physicsObject;

  auto colliderB =
      e2->GetComponent<Component::CCollider>()->collider->UsingOrigin(
          objectB->GetPosition());

  Physics::IntersectData id =
      Physics::Collider::Intersect(*colliderA, *colliderB);

  physicsEngine.AddCollision(Physics::Collision{
      .intersection = id, .objectA = objectA, .objectB = objectB});
}

void SPhysicsSystem::update(std::shared_ptr<Core::Entity> entity, float ts) {
  physicsEngine.Simulate(
      entity->GetComponent<Component::CPhysicsObject>()->physicsObject, ts);
}

void SPhysicsSystem::resolveCollisions() { physicsEngine.ResolveCollisions(); }

void SPhysicsSystem::sync(std::shared_ptr<Core::Entity> e) {
  auto t = e->GetComponent<Component::CTransform>();
  auto p = e->GetComponent<Component::CPhysicsObject>();
  t->transform.setPosition(p->physicsObject->GetPosition());
}
