#pragma once
#include "GLFW/glfw3.h"
#include "event.h"
#include <string>
#include "glm/vec2.hpp"

namespace Core {
struct WindowSpecification {
  std::string Title;
  uint32_t Width = 1280;
  uint32_t Height = 720;
  bool IsResizeable = true;
  bool VSync = true;
  using EventCallbackFn = std::function<void(Event &)>;
  EventCallbackFn EventCallback;
};

class Window {
public:
  Window(const WindowSpecification &specification = WindowSpecification());
  ~Window();

  void Create();
  void Destroy();

  void Update();

  void RaiseEvent(Event &event);

  glm::vec2 GetFrameBufferSize() const;
  glm::vec2 GetMousePosition() const;

  bool ShouldClose() const;

  GLFWwindow *GetHandle() const { return m_Handle; }

private:
  WindowSpecification m_Specification;
  GLFWwindow *m_Handle = nullptr;
  double mouseX, mouseY;
  bool firstMove = true;
};

} // namespace Core
