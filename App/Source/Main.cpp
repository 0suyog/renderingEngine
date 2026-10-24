#include "AppLayer.h"
#include "Font/font.h"
#include "Game/gameLayer.h"
#include "application.h"
#include <iostream>
#include <memory>
// DEBUG: main function is being called twice when gameLayer->setCurrenScene is
// called
int main() {

  auto appSpec = Core::ApplicationSpecification();
  appSpec.Name = "Test Application";

  auto app = Core::Application(appSpec);
  app.PushLayer<Game::GameLayer>();
  auto gameLayer = app.GetLayer<Game::GameLayer>();
  gameLayer->setCurrentScene(std::make_shared<Playground>());
  app.Run();
}
