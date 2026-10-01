#pragma once

#include "Game/scene.h"
#include "GLFW/glfw3.h"
#include "Physics/aabb.h"
#include "Renderer/camera.h"
#include "Renderer/texture.h"
#include "action.h"
#include "application.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include <iostream>
#include <memory>

class Playground : public Game::Scene {
public:
  Playground() : Game::Scene() {
    renderContext = {
        .cam = std::make_shared<Renderer::Camera>(),
        .viewPortWidth = 600,
        .viewPortHeight = 800,
    };
    auto floor = entityManager.AddEntity("floor");
    floor
        ->addCShape(Renderer::Cuboid(
            500, 3, 500,
            Renderer::Material{.diffuseTexture = Renderer::GetBuiltInTexture(
                                   Renderer::BuiltInTexture::DEFAULT),
                               .specularTexture = Renderer::GetBuiltInTexture(
                                   Renderer::BuiltInTexture::WHITE),
                               .shininess = 64}))
        .addCTransform(Core::Transform())
        .addCPhysicsObject(
            physicsEngine.AddObject({0, 0, 0}, {0, 0, 0}, false, true))
        .addCCollider(Physics::Aabb::Generate(floor->cShape->shape.m_meshes))
        .addCStatic();

    auto aabb = entityManager.AddEntity("aabb");
    aabb->addCShape(
            Renderer::Cube(5,
                           Renderer::Material{
                               .diffuseTexture = Renderer::GetBuiltInTexture(
                                   Renderer::BuiltInTexture::DEFAULT),
                           }))
        .addCTransform(Core::Transform().moveto({0, 0, 0}))
        .addCPhysicsObject(physicsEngine.AddObject({0, 10, 0}, {0, 0, 0}))
        .addCCollider(Physics::Aabb::Generate(aabb->cShape->shape.m_meshes))
        .addCSpeed(10, 20)
        .addCCamera(renderContext.cam, 40)
        .addCLight(Renderer::PointLight({100, 100, 100}, 0.9, 0.32));
    entityManager.AddEntity("sun")->addCLight(
        Renderer::DirectionalLight({0.5, 0.5, 0.5}, {0.5, 0.5, 0.0}));
    player = aabb;
    physicsEngine.gravity.y = -2;
    setUpActions();
  }
  void update(float ts) override {
    entityManager.update();
    physicsEngine.Simulate(ts);
    renderContext.lights = {};

    const auto &entities = entityManager.GetEntities();

    for (unsigned int i = 0; i < entities.size(); i++) {

      if (entities[i]->cPhysicsObject && entities[i]->cCollider) {

        std::shared_ptr<Physics::PhysicsObject> objectA =
            entities[i]->cPhysicsObject->physicsObject;

        auto colliderA = entities[i]->cCollider->collider->UsingOrigin(
            objectA->GetPosition());

        for (unsigned int j = i + 1; j < entities.size(); j++) {

          if (entities[j]->cPhysicsObject && entities[j]->cCollider) {

            std::shared_ptr<Physics::PhysicsObject> objectB =
                entities[j]->cPhysicsObject->physicsObject;

            auto colliderB = entities[j]->cCollider->collider->UsingOrigin(
                objectB->GetPosition());

            Physics::IntersectData id =
                Physics::Collider::Intersect(*colliderA, *colliderB);

            physicsEngine.AddCollision(Physics::Collision{
                .intersection = id, .objectA = objectA, .objectB = objectB});
          }
        }
      }
    }
    physicsEngine.ResolveCollisions();
    for (const auto &entity : entities) {
      if (entity->cPhysicsObject && entity->cTransform) {
        entity->cTransform->transform.moveto(
            entity->cPhysicsObject->physicsObject->GetPosition());
      }

      if (entity->cLight) {
        if (entity->cTransform) {
          renderContext.lights.push_back(entity->cLight->light.UsingOrigin(
              entity->cTransform->transform.getPosition()));
        } else {
          renderContext.lights.push_back(entity->cLight->light);
        }
      }
    }
    if (player->cCamera) {
      player->cCamera->camera->follow(
          player->cTransform->transform.getPosition(),
          player->cCamera->followDistance);
    }

    if (player->cSpeed) {
      auto playerMovementVector = glm::vec3(0);
      auto speed = player->cSpeed->walkSpeed;
      // // move camera based on activeActions
      if (activeActions[Core::Action::FORWARD]) {
        playerMovementVector += renderContext.cam->getForward();
      }
      if (activeActions[Core::Action::BACKWARD]) {
        playerMovementVector -= renderContext.cam->getForward();
      }
      if (activeActions[Core::Action::LEFT]) {
        playerMovementVector -= renderContext.cam->getRight();
      }

      if (activeActions[Core::Action::RIGHT]) {
        playerMovementVector += renderContext.cam->getRight();
      }

      if (activeActions[Core::Action::FAST]) {
        speed = player->cSpeed->sprintSpeed;
      }

      if (!(playerMovementVector == glm::vec3(0))) {
        playerMovementVector = glm::normalize(playerMovementVector);
        auto &playerPhysicsObject = player->cPhysicsObject->physicsObject;
        playerPhysicsObject->SetVelocity(playerPhysicsObject->GetVelocity() +
                                         speed * playerMovementVector * ts);
      }

      if (activeActions[Core::Action::JUMP]) {
        player->cPhysicsObject->physicsObject->SetVelocity(
            player->cPhysicsObject->physicsObject->GetVelocity() +
            glm::vec3(0, 10, 0));
      }
      if (activeActions[Core::Action::DOWN]) {
        player->cPhysicsObject->physicsObject->SetVelocity(
            player->cPhysicsObject->physicsObject->GetVelocity() -
            glm::vec3(0, 10, 0));
      }
    }
  }

  void onMouseMove(float dx, float dy) override {
    renderContext.cam->rotateByMouse(dx, dy);
  }

  void setUpActions() {
    registerAction(GLFW_KEY_CAPS_LOCK, Core::Action::QUIT);
    registerAction(GLFW_KEY_W, Core::Action::FORWARD);
    registerAction(GLFW_KEY_S, Core::Action::BACKWARD);
    registerAction(GLFW_KEY_A, Core::Action::LEFT);
    registerAction(GLFW_KEY_D, Core::Action::RIGHT);
    registerAction(GLFW_KEY_LEFT_SHIFT, Core::Action::DOWN);
    registerAction(GLFW_KEY_SPACE, Core::Action::JUMP);
    registerAction(GLFW_KEY_LEFT_CONTROL, Core::Action::FAST);
  }

  void doAction(const Core::Action &action) override {
    if (action.isStart) {
      if (action.type == Core::Action::QUIT) {
        auto &app = Core::Application::Get();
        app.Stop();
      }
      if (action.type == Core::Action::FORWARD) {
        activeActions[Core::Action::FORWARD] = true;
      }
      if (action.type == Core::Action::BACKWARD) {
        activeActions[Core::Action::BACKWARD] = true;
      }
      if (action.type == Core::Action::FAST) {
        activeActions[Core::Action::FAST] = true;
      }
      if (action.type == Core::Action::LEFT) {
        activeActions[Core::Action::LEFT] = true;
      }
      if (action.type == Core::Action::RIGHT) {
        activeActions[Core::Action::RIGHT] = true;
      }
      if (action.type == Core::Action::JUMP) {
        activeActions[Core::Action::JUMP] = true;
      }
      if (action.type == Core::Action::DOWN) {
        activeActions[Core::Action::DOWN] = true;
      }
    } else {
      if (action.type == Core::Action::FORWARD) {
        activeActions[Core::Action::FORWARD] = false;
      }
      if (action.type == Core::Action::BACKWARD) {
        activeActions[Core::Action::BACKWARD] = false;
      }
      if (action.type == Core::Action::RIGHT) {
        activeActions[Core::Action::RIGHT] = false;
      }
      if (action.type == Core::Action::LEFT) {
        activeActions[Core::Action::LEFT] = false;
      }
      if (action.type == Core::Action::FAST) {
        activeActions[Core::Action::FAST] = false;
      }
      if (action.type == Core::Action::JUMP) {
        activeActions[Core::Action::JUMP] = false;
      }
      if (action.type == Core::Action::DOWN) {
        activeActions[Core::Action::DOWN] = false;
      }
    }
  }

private:
  bool isCamMoving = false;
};
