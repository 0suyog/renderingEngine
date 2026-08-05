#include <glad/glad.h>
#include "event.h"
#include "windowevents.h"
#include "inputevents.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>
namespace Core {
Window::Window(const WindowSpecification &specification)
    : m_Specification(specification) {}
Window::~Window() { Destroy(); }

void Window::Create() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

  m_Handle = glfwCreateWindow(m_Specification.Width, m_Specification.Height,
                              m_Specification.Title.c_str(), NULL, NULL);

  if (!m_Handle) {
    std::cerr << "Failde to create GLFW window\n";
    glfwTerminate();
    exit(-1);
  }

  glfwMakeContextCurrent(m_Handle);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    glfwTerminate();
    exit(-1);
  }

  glfwSwapInterval(m_Specification.VSync ? 1 : 0);
  glfwSetWindowUserPointer(m_Handle, this);

  glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow *handle) {
    Window &window = *((Window *)glfwGetWindowUserPointer(handle));
    WindowClosedEvent event;
    window.RaiseEvent(event);
  });

  glfwSetWindowSizeCallback(
      m_Handle, [](GLFWwindow *handle, int width, int height) {
        Window &window = *((Window *)glfwGetWindowUserPointer(handle));
        WindowResizeEvent event(width, height);
        window.RaiseEvent(event);
      });

  glfwSetMouseButtonCallback(
      m_Handle, [](GLFWwindow *handle, int key, int action, int modifiers) {
        Window &window = *((Window *)glfwGetWindowUserPointer(handle));
        if (action == GLFW_PRESS) {
          MouseButtonPressedEvent e(key);
          window.RaiseEvent(e);
          return;
        }
        MouseButtonReleasedEvent e(key);
        window.RaiseEvent(e);
      });

  glfwSetKeyCallback(m_Handle, [](GLFWwindow *handle, int key, int scanCode,
                                  int action, int mods) {
    Window &window = *((Window *)glfwGetWindowUserPointer(handle));
    switch (action) {
    case GLFW_PRESS:
    case GLFW_REPEAT: {
      KeyPressedEvent e(scanCode, key, action == GLFW_REPEAT);
      window.RaiseEvent(e);
      break;
    }
    case GLFW_RELEASE: {
      KeyReleasedEvent e(scanCode, key);
      window.RaiseEvent(e);
      break;
    }
    }
  });
}

void Window::Destroy() {
  if (m_Handle) {
    glfwDestroyWindow(m_Handle);
  }
  m_Handle = nullptr;
}

void Window::Update() { glfwSwapBuffers(m_Handle); }

bool Window::ShouldClose() const { return glfwWindowShouldClose(m_Handle); }

void Window::RaiseEvent(Event &event) {
  if (m_Specification.EventCallback) {
    m_Specification.EventCallback(event);
  }
}

glm::vec2 Window::GetFrameBufferSize() const {
  int width, height;
  glfwGetWindowFrameSize(m_Handle, 0, 0, &width, &height);
  return {width, height};
}

glm::vec2 Window::GetMousePosition() const { return {0, 0}; }

} // namespace Core
