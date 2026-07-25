#include "application.h"
#include "AppLayer.h"
int main() {
  auto appSpec = Core::ApplicationSpecification();
  appSpec.Name = "Test Application";

  auto app = Core::Application(appSpec);
  app.PushLayer<RedLayer>();
  app.Run();
}
