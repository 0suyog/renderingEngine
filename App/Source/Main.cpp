#include "application.h"
int main() {
  auto appSpec = Core::ApplicationSpecification();
  appSpec.Name = "Test Application";

  auto app = Core::Application(appSpec);
  app.Run();
}
