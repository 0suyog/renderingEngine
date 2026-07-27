#pragma once
#include "glad/glad.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
namespace Renderer {
class ShaderProgram {

public:
  ShaderProgram(const std::string &vertexPath, const std::string &fragmentPath);
  void reload();

private:
  GLuint m_Handle;
  std::string m_vertexPath;
  std::string m_fragmentPath;

  GLuint init();
};
} // namespace Renderer
