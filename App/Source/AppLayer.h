#pragma once
#include "Physics/aabb.h"
#include "Physics/collider.h"
#include "Physics/collision.h"
#include "Physics/intersectData.h"
#include "Physics/physicsObject.h"
#include "Renderer/camera.h"
#include "Renderer/mesh.h"
#include "Renderer/renderer.h"
#include "Renderer/shapes3d.h"
#include "Renderer/texture.h"
#include "entityManager.h"
#include "Renderer/renderContext.h"
#include "entity.h"
#include "transform.h"
#include "windowevents.h"
#include "action.h"
#include "Renderer/shader.h"
#include "application.h"
#include "event.h"
#include "layer.h"
#include "inputevents.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "Physics/physicsEngine.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
class AppLayer : public Core::Layer {
public:
  AppLayer() {
    auto floor = entityManager.AddEntity("floor");
    floor
        ->addCShape(Renderer::Cuboid(
            100, 5, 200,
            Renderer::Material{.diffuseTexture = Renderer::GetBuiltInTexture(
                                   Renderer::BuiltInTexture::DEFAULT)}))
        .addCTransform(Core::Transform())
        .addCPhysicsObject(
            physicsEngine.AddObject({0, 0, 0}, {0, 0, 0}, false, true))
        .addCCollider(Physics::Aabb::Generate(floor->cShape->shape.m_meshes))
        .addCStatic();

    auto aabb = entityManager.AddEntity("aabb");
    aabb
        ->addCShape(Renderer::Cube(
            3, Renderer::Material{.diffuseTexture = Renderer::GetBuiltInTexture(
                                      Renderer::BuiltInTexture::DEFAULT)}))
        .addCTransform(Core::Transform().moveto({0, 0, 0}))
        .addCPhysicsObject(physicsEngine.AddObject({0, 10, 0}, {0, 0, 0}))
        .addCCollider(Physics::Aabb::Generate(aabb->cShape->shape.m_meshes));

    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(),
                     GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cam->position({0, 4, 10});
    physicsEngine.gravity.y = -2;
  }

  void OnUpdate(float ts) override {
    physicsEngine.isPaused = isPaused;
    entityManager.update();
    physicsEngine.Simulate(ts);

    const auto &entities = entityManager.GetEntities();
    if (!isPaused) {
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
              auto coll = Physics::Collision{
                  .intersection = id, .objectA = objectA, .objectB = objectB};
              if (coll.intersection.GetDoesIntersect()) {
                physicsEngine.AddCollision(coll);
              }
            }
          }
        }
      }
    }

    physicsEngine.ResolveCollisions();

    for (const auto &entity : entities) {
      if (entity->cPhysicsObject) {
        entity->cTransform->transform.moveto(
            entity->cPhysicsObject->physicsObject->GetPosition());
      }
    }

    // Core::Action camMovement = Core::NONE;
    //
    // if (Core::KeyEvent::GetKey(GLFW_KEY_W) == GLFW_PRESS) {
    //   camMovement = static_cast<Core::Action>(camMovement | Core::FORWARD);
    // }
    // if (Core::KeyEvent::GetKey(GLFW_KEY_S) == GLFW_PRESS) {
    //   camMovement = static_cast<Core::Action>(camMovement | Core::BACKWARD);
    // }
    // if (Core::KeyEvent::GetKey(GLFW_KEY_A) == GLFW_PRESS) {
    //   camMovement = static_cast<Core::Action>(camMovement | Core::LEFT);
    // }
    // if (Core::KeyEvent::GetKey(GLFW_KEY_D) == GLFW_PRESS) {
    //   camMovement = static_cast<Core::Action>(camMovement | Core::RIGHT);
    // }
    // if (Core::KeyEvent::GetKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    //   camMovement = static_cast<Core::Action>(camMovement | Core::DOWN);
    // }
    // if (Core::KeyEvent::GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
    //   camMovement = static_cast<Core::Action>(camMovement | Core::UP);
    // }
    // if (Core::KeyEvent::GetKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
    //   camMovement = static_cast<Core::Action>(camMovement | Core::FAST);
    // }
    //
    // cam->move(camMovement, ts);
  }

  void OnRender() override {
    // glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, cam->getWidth(), cam->getHeight());
    for (const auto &entity : entityManager.GetEntities()) {
      if (entity->cShape && entity->cTransform) {
        auto modelMatrix = entity->cTransform->transform.matrix();
        Renderer::render(renderContext, entity->cShape->shape, modelMatrix);
      }
    }
  }

  void OnEvent(Core::Event &e) override {
    // std::cerr << "[Event Occured]: " << e.ToString() << std::endl;
    Core::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<Core::MouseButtonPressedEvent>(
        [this](Core::MouseButtonPressedEvent &e) {
          return handleMousePress(e);
        });
    dispatcher.Dispatch<Core::KeyPressedEvent>(
        [this](Core::KeyPressedEvent &e) { return handleKeyPress(e); });
    dispatcher.Dispatch<Core::WindowResizeEvent>(
        [this](Core::WindowResizeEvent &e) { return handleWindowResize(e); });
    dispatcher.Dispatch<Core::MouseMovedEvent>(
        [this](Core::MouseMovedEvent &e) { return handleMouseMove(e); });
  }

private:
  bool handleMousePress(Core::MouseButtonPressedEvent &e) {
    auto box = entityManager.GetEntities("aabb")[0];
    box->cPhysicsObject->physicsObject->applyForce(cam->getForward() * 100.0f);
    return true;
  }

  bool handleKeyPress(Core::KeyPressedEvent &e) {
    // std::cerr << "char: " << e.GetPrintableChar() << '\n';
    if (e.GetKeyCode() == GLFW_KEY_ESCAPE ||
        e.GetKeyCode() == GLFW_KEY_CAPS_LOCK) {
      auto &app = Core::Application::Get();
      app.Stop();
    }

    if (e.GetKeyCode() == GLFW_KEY_R) {
      isPaused = !isPaused;
    }

    return true;
  }

  bool handleWindowResize(Core::WindowResizeEvent &e) {
    cam->setWidth(e.GetWidth());
    cam->setHeight(e.GetHeight());
    return false;
  }

  bool handleMouseMove(Core::MouseMovedEvent &e) {
    cam->rotateByMouse(e.GetdX(), e.GetdY());
    // std::cerr << "dx: " << e.GetdX() << "dy: " << e.GetdY() << "\n";
    return false;
  }

private:
  std::shared_ptr<Renderer::Camera> cam = std::make_shared<Renderer::Camera>();
  Renderer::RenderContext renderContext = {this->cam};
  Core::EntityManager entityManager;
  Physics::PhysicsEngine physicsEngine;
  bool isPaused = false;
};
