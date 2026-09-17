#pragma once
#include "Physics/boundingSphere.h"
#include "Physics/collider.h"
#include "Physics/collision.h"
#include "Physics/intersectData.h"
#include "Physics/physicsObject.h"
#include "Physics/plane.h"
#include "Renderer/camera.h"
#include "Renderer/regularPolygon.h"
#include "Renderer/renderer.h"
#include "entityManager.h"
#include "Renderer/renderContext.h"
#include "entity.h"
#include "transform.h"
#include "windowevents.h"
#include "movement.h"
#include "Renderer/shader.h"
#include "application.h"
#include "event.h"
#include "layer.h"
#include "inputevents.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include "Physics/physicsEngine.h"
class AppLayer : public Core::Layer {
public:
  AppLayer() {
    auto wall = entityManager.AddEntity("wall");
    wall->addCShape(Renderer::RegularPolygon(15, 4, {0.38, 0.6, 0.89, 1.0}, 0.0,
                                             {0, 1, 0}))
        .addCTransform(Core::Transform())
        .addCPhysicsObject(
            physicsEngine.AddObject({0, 0, 0}, {0, 0, 0}, false, true))
        .addCCollider(std::make_shared<Physics::Plane>(glm::vec3({0, 1, 0}),
                                                       glm::vec3({0, 0, 0})))
        .addCStatic();

    auto ball = entityManager.AddEntity("ball");
    ball->addCShape(
            Renderer::RegularPolygon(4, 32, {1.0, 0, 0, 0.5}, 0.0, {0, 0, 1}))
        .addCTransform(Core::Transform().moveto({0, 10, 0}))
        .addCPhysicsObject(physicsEngine.AddObject({0, 10, 0}, {0, 0, 0}))
        .addCCollider(
            std::make_shared<Physics::BoundingSphere>(glm::vec3{0, 0, 0}, 5));

    glfwSetInputMode(Core::Application::Get().GetWindow()->GetHandle(),
                     GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cam->position({0, 0, 0});
  }

  void OnUpdate(float ts) override {

    entityManager.update();
    physicsEngine.Simulate(ts);

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
      if (entity->cPhysicsObject) {
        entity->cTransform->transform.moveto(
            entity->cPhysicsObject->physicsObject->GetPosition());
      }
    }

    Core::Movement camMovement = Core::NONE;

    if (Core::KeyEvent::GetKey(GLFW_KEY_W) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::FORWARD);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_S) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::BACKWARD);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_A) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::LEFT);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_D) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::RIGHT);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::DOWN);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::UP);
    }
    if (Core::KeyEvent::GetKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
      camMovement = static_cast<Core::Movement>(camMovement | Core::FAST);
    }

    cam->move(camMovement, ts);
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
    std::cerr << "button: " << e.GetMouseButton() << "\n";
    return true;
  }

  bool handleKeyPress(Core::KeyPressedEvent &e) {
    // std::cerr << "char: " << e.GetPrintableChar() << '\n';
    if (e.GetKeyCode() == GLFW_KEY_ESCAPE ||
        e.GetKeyCode() == GLFW_KEY_CAPS_LOCK) {
      auto &app = Core::Application::Get();
      app.Stop();
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
};
