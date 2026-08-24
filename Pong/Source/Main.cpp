#include "gamelayer.h"
#include "application.h"
int main() {
  auto appSpec = Core::ApplicationSpecification();
  appSpec.Name = "Pong";
  auto application = Core::Application();
  application.PushLayer<GameLayer>();
  application.Run();
}
