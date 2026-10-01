#include "AppLayer.h"
#include "Game/gameLayer.h"
#include "application.h"
#include <memory>
int main() {
  auto appSpec = Core::ApplicationSpecification();
  appSpec.Name = "Test Application";

  auto app = Core::Application(appSpec);
  app.PushLayer<Game::GameLayer>();
  auto gameLayer = app.GetLayer<Game::GameLayer>();
  gameLayer->setCurrenScene(std::make_shared<Playground>());
  app.Run();
}
