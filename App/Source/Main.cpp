#include "AppLayer.h"
#include "Font/font.h"
#include "Game/gameLayer.h"
#include "application.h"
#include <iostream>
#include <memory>
// DEBUG: main function is being called twice when gameLayer->setCurrenScene is
// called
int main() {

  // Core::WriteTestDefault();

  // Core::Font f = Core::Font(
  //     "/usr/share/fonts/IsovekaTerm/IosevkaTermNerdFontMono-Bold.ttf");
  auto appSpec = Core::ApplicationSpecification();
  appSpec.Name = "Test Application";

  std::cerr << "size: " << sizeof(unsigned char) << "\n";

  auto app = Core::Application(appSpec);
  app.PushLayer<Game::GameLayer>();
  auto gameLayer = app.GetLayer<Game::GameLayer>();
  gameLayer->setCurrentScene(std::make_shared<Playground>());
  app.Run();
}
