#pragma once

#include "Components/CPhysicsObject.h"
#include "Components/CShape.h"
#include "Components/CTransform.h"
#include "Game/scene.h"
#include "GLFW/glfw3.h"
#include "Physics/aabb.h"
#include "Physics/physicsObject.h"
#include "Renderer/camera.h"
#include "Renderer/texture.h"
#include "action.h"
#include "application.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "Font/font.h"
#include <cstdint>
#include <memory>
#include <vector>

class Playground : public Game::Scene {
public:
  Playground() : Game::Scene() {

    font = // Core::loadFont(
           //"/usr/share/fonts/IsovekaTerm/IosevkaTermNerdFont-Heavy.ttf");
        // Core::loadFont("/usr/share/fonts/noto/NotoSerifDevanagari-Medium.ttf");
        // Core::loadFont("/usr/share/fonts/noto/NotoSerifDevanagari-Regular.ttf");
        Core::loadFont("../assets/fonts/Poppins-Regular.ttf");
    renderContext = {
        .cam = std::make_shared<Renderer::Camera>(),
        .viewPortWidth = 600,
        .viewPortHeight = 800,
    };

    // auto floor = entityManager.AddEntity("floor");
    //
    // floor
    //     ->addComponent<Component::CShape>(Renderer::Cuboid(
    //         500, 3, 500,
    //         Renderer::Material{
    //             .diffuseTexture = font->getAtlasTexture(150, 150),
    //             .specularTexture = font->getAtlasTexture(150, 150),
    //             .shininess = 64}))
    //     .addComponent<Component::CTransform>(Core::Transform())
    //     .addComponent<Component::CPhysicsObject>(
    //         std::make_shared<Physics::PhysicsObject>(
    //             glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), false, true))
    //     .addComponent<Component::CCollider>(Physics::Aabb::Generate(
    //         floor->GetComponent<Component::CShape>()->shape.m_meshGeometries))
    //     .addComponent<Component::CStatic>();

    auto aabb = entityManager.AddEntity("aabb");
    //
    aabb->addComponent<Component::CShape>(
            Renderer::Cube(5,
                           Renderer::Material{
                               .diffuseTexture = Renderer::GetBuiltInTexture(
                                   Renderer::BuiltInTexture::DEFAULT),
                               .specularTexture = Renderer::GetBuiltInTexture(
                                   Renderer::BuiltInTexture::DEFAULT),
                           }))
        .addComponent<Component::CTransform>(
            Core::Transform().setPosition({0, 0, 0}))
        .addComponent<Component::CPhysicsObject>(
            std::make_shared<Physics::PhysicsObject>(glm::vec3(0, 10, 0),
                                                     glm::vec3(0, 0, 0), false))
        .addComponent<Component::CCollider>(Physics::Aabb::Generate(
            aabb->GetComponent<Component::CShape>()->shape.m_meshGeometries))
        .addComponent<Component::CSpeed>(10, 20)
        .addComponent<Component::CCamera>(renderContext.cam, 40)
        .addComponent<Component::CLight>(
            Renderer::PointLight({100, 100, 100}, 0.9, 0.32));

    entityManager.AddEntity("sun")->addComponent<Component::CLight>(
        Renderer::DirectionalLight({2, 2, 2}, {0.5, 0.5, 0.0}));

    player = aabb;

    // std::string c = " ि";

    textManager.add(font, R"(क – कपुरि क  apple
ख – खरायो ख
ग – गाईगोड़े ग
घ – घर घ
ङ – ङ
च – चरा च
छ – छाता छ
ज – जहाज ज
झ – झण्डा झ
ञ – ञ
ट – टमाटर ट
ठ – ठेला ठ
ड – डमरु ड
ढ – ढोका ढ
ण – ण
त – तरकारी त
थ – थर्मस थ
द – दाँत द
ध – धनुष ध
न – नल न
प – पतंग प
फ – फल फ
ब – बाघ ब
भ – भालु भ
म – माछा म
य – यातायात य
र – रथ र
ल – लट्टाई ल
व – वन व
श – शङ्ख श
ष – षट्कोण ष
स – स्याउ स
ह – हात्ती ह
क्ष – क्षितिज क्ष
त्र – त्रिशूल त्र
ज्ञ – ज्ञानी ज्ञ
अ – अनार अ
आ – आँप आ
इ – इमली इ
ई – ईश्वर ई
उ – उल्लु उ
ऊ – ऊन ऊ
ऋ – ऋषि ऋ
ए – एक ए
ऐ – ऐना ऐ
ओ – ओखर ओ
औ – औषधि औ
अं – अंगुर अं
अः – दुःख अः

ॐ – ॐ
श्री – श्री
    )",
                    Core::Transform().setPosition(glm::vec3(0)), {1, 0, 0, 1},
                    {1, 1, 1, 0});

    // textManager.add(font, "a\nb",
    // Core::Transform().setPosition(glm::vec3(0)));

    sPhysics.setGravity(-2);

    setUpActions();
  }

  void update(float ts) override {
    renderContext.lights = {};

    entityManager.update();
    const auto &entities = entityManager.GetEntities();

    for (unsigned int i = 0; i < entities.size(); i++) {
      if (entities[i]
              ->HasComponents<Component::CPhysicsObject, Component::CTransform,
                              Component::CCollider>()) {
        sPhysics.update(entities[i], ts);
      }

      for (unsigned int j = i + 1; j < entities.size(); j++) {
        if (entities[j]
                ->HasComponents<Component::CPhysicsObject,
                                Component::CCollider>()) {
          sPhysics.detectCollision(entities[i], entities[j]);
        }
      }
    }

    sPhysics.resolveCollisions();

    for (const auto &entity : entities) {
      if (entity->HasComponents<Component::CPhysicsObject,
                                Component::CTransform>()) {
        sPhysics.sync(entity);
      }

      if (entity->HasComponents<Component::CLight>()) {
        auto light = entity->GetComponent<Component::CLight>()->light;

        if (entity->HasComponents<Component::CTransform>()) {
          renderContext.lights.push_back(
              light.UsingOrigin(entity->GetComponent<Component::CTransform>()
                                    ->transform.getPosition()));
        } else {
          renderContext.lights.push_back(light);
        }
      }
    }

    if (player->HasComponents<Component::CCamera, Component::CTransform>()) {

      auto camera = player->GetComponent<Component::CCamera>();

      auto transform = player->GetComponent<Component::CTransform>();

      camera->camera->follow(transform->transform.getPosition(),
                             camera->followDistance);
    }

    if (player->HasComponents<Component::CSpeed, Component::CPhysicsObject>()) {

      auto speedComponent = player->GetComponent<Component::CSpeed>();

      auto physicsObject =
          player->GetComponent<Component::CPhysicsObject>()->physicsObject;

      auto playerMovementVector = glm::vec3(0);

      auto speed = speedComponent->walkSpeed;

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
        speed = speedComponent->sprintSpeed;
      }

      if (playerMovementVector != glm::vec3(0)) {
        playerMovementVector = glm::normalize(playerMovementVector);

        physicsObject->SetPosition(physicsObject->GetPosition() +
                                   speed * playerMovementVector * ts);
      }

      if (activeActions[Core::Action::JUMP]) {
        physicsObject->SetPosition(physicsObject->GetPosition() +
                                   glm::vec3(0, 5, 0) * ts);
      }

      if (activeActions[Core::Action::DOWN]) {
        physicsObject->SetPosition(physicsObject->GetPosition() -
                                   glm::vec3(0, 15, 0) * ts);
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

      if (action.type == Core::Action::FORWARD)
        activeActions[Core::Action::FORWARD] = true;

      if (action.type == Core::Action::BACKWARD)
        activeActions[Core::Action::BACKWARD] = true;

      if (action.type == Core::Action::FAST)
        activeActions[Core::Action::FAST] = true;

      if (action.type == Core::Action::LEFT)
        activeActions[Core::Action::LEFT] = true;

      if (action.type == Core::Action::RIGHT)
        activeActions[Core::Action::RIGHT] = true;

      if (action.type == Core::Action::JUMP)
        activeActions[Core::Action::JUMP] = true;

      if (action.type == Core::Action::DOWN)
        activeActions[Core::Action::DOWN] = true;

    } else {
      if (action.type == Core::Action::FORWARD)
        activeActions[Core::Action::FORWARD] = false;

      if (action.type == Core::Action::BACKWARD)
        activeActions[Core::Action::BACKWARD] = false;

      if (action.type == Core::Action::RIGHT)
        activeActions[Core::Action::RIGHT] = false;

      if (action.type == Core::Action::LEFT)
        activeActions[Core::Action::LEFT] = false;

      if (action.type == Core::Action::FAST)
        activeActions[Core::Action::FAST] = false;

      if (action.type == Core::Action::JUMP)
        activeActions[Core::Action::JUMP] = false;

      if (action.type == Core::Action::DOWN)
        activeActions[Core::Action::DOWN] = false;
    }
  }

private:
  std::shared_ptr<Core::Font> font;
};
