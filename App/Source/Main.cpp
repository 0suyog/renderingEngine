#include "AppLayer.h"
#include "Game/gameLayer.h"
#include "application.h"
#include <memory>
#include <fmt/format.h>
#include <fmt/color.h>
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
