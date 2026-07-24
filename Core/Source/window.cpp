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

glm::vec2 Window::GetFrameBufferSize() const {
  int width, height;
  glfwGetWindowFrameSize(m_Handle, 0, 0, &width, &height);
  return {width, height};
}

glm::vec2 Window::GetMousePosition() const { return {0, 0}; }

} // namespace Core
