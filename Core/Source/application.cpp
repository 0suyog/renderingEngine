#include "application.h"
#include <GLFW/glfw3.h>
#include "glm/common.hpp"
#include <iostream>
#include <ranges>
namespace Core {

static Application *s_Application = nullptr;

static void GLFWErrorCallback(int error, const char *description) {
  std::cerr << "[GLFW Error]: " << description << std::endl;
}

Application::Application(const ApplicationSpecification &spec)
    : m_Specification(spec) {
  s_Application = this;
  glfwSetErrorCallback(GLFWErrorCallback);
  glfwInit();

  if (m_Specification.WindowSpec.Title.empty()) {
    m_Specification.WindowSpec.Title = m_Specification.Name;
  }

  m_Specification.WindowSpec.EventCallback = [this](Event &event) {
    RaiseEvent(event);
  };

  m_Window = std::make_shared<Window>(m_Specification.WindowSpec);
  m_Window->Create();
}

Application::~Application() {
  m_Window->Destroy();
  glfwTerminate();
  s_Application = nullptr;
}

void Application::Run() {
  m_Running = true;
  float lastTime = GetTime();
  while (m_Running) {
    glfwPollEvents();

    if (m_Window->ShouldClose()) {
      Stop();
      break;
    }
    float currentTime = GetTime();
    float timestep = glm::clamp(currentTime - lastTime, 0.001f, 0.1f);
    lastTime = currentTime;

    for (auto &layer : m_LayerStack) {
      layer->OnUpdate(timestep);
      layer->OnRender();
    }

    m_Window->Update();
  }
}

void Application::Stop() { m_Running = false; }

void Application::RaiseEvent(Event &event) {
  for (auto &layer : std::views::reverse(m_LayerStack)) {
    layer->OnEvent(event);
    if (event.Handled) {
      break;
    }
  }
}

glm::vec2 Application::GetFramebufferSize() const {
  return m_Window->GetFrameBufferSize();
}

Application &Application::Get() {
  assert(s_Application);
  return *s_Application;
}

float Application::GetTime() { return (float)glfwGetTime(); }

} // namespace Core
