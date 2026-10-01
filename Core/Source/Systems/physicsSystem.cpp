#include "physicsSystem.h"

void SPhysicsSystem::collision(std::shared_ptr<Core::Entity> e1,
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

void SPhysicsSystem::update(float ts) {
  physicsEngine.Simulate(ts);
  physicsEngine.ResolveCollisions();
}

void SPhysicsSystem::sync(std::shared_ptr<Core::Entity> e) {
  auto t = e->GetComponent<Component::CTransform>();
  auto p = e->GetComponent<Component::CPhysicsObject>();
  t->transform.setPosition(p->physicsObject->GetPosition());
}
