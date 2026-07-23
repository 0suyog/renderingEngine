#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string>

namespace Core {

class Application {

public:
  Application(int width, int height, std::string title)
      : windowWidth(width), windowHeight(height), title(title) {
    Init();
    AppLoop();
  }

private:
  int windowWidth, windowHeight;
  std::string title;
  GLFWwindow *window;
  void Init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window =
        glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);

    if (!window) {
      std::cerr << "Failed Creating glfw window exitting" << std::endl;
      glfwTerminate();
      exit(-1);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Failed to initialize GLAD" << std::endl;
      glfwTerminate();
      exit(-1);
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  }

  void AppLoop() {
    while (!glfwWindowShouldClose(window)) {
      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  static void framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height) {
    glViewport(0, 0, width, height);
  }

  static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods) {}
};

} // namespace Core
