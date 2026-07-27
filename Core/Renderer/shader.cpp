#include "shader.h"
#include "file.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <algorithm>
#include <iostream>
#include <linux/input.h>
#include <vector>
namespace Renderer {

ShaderProgram::ShaderProgram(const std::string &vertexPath,
                             const std::string &fragmentPath)
    : m_vertexPath(vertexPath), m_fragmentPath(fragmentPath) {
  auto programId = init();
  if (programId != 0) {
    m_Handle = programId;
  }
}

GLuint ShaderProgram::init() {
  const char *vertexSource = Core::ReadTextFile(m_vertexPath);
  if (vertexSource == nullptr) {
    std::cerr << "[SHADER] cannot open vertex shader file";
    return -1;
  }

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, 0);

  GLint status = 0;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

  if (status == GL_FALSE) {
    GLint maxLength = 0;
    std::cerr << "[SHADER] Vertex shader compilation failed" << "\n";
    std::vector<GLchar> infoLog(maxLength);
    std::cerr << infoLog.data() << "\n";
    glDeleteShader(vertexShader);
    return -1;
  }

  const char *fragmentSource = Core::ReadTextFile(m_fragmentPath);

  if (fragmentSource == nullptr) {
    std::cerr << "[SHADER] cannot open fragment shader file";
    return -1;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, 0);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint maxLength = 0;
    std::cerr << "[SHADER] Fragment shader compilation failed" << "\n";
    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
    std::cerr << infoLog.data() << "\n";
    glDeleteShader(fragmentShader);
    return -1;
  }

  GLuint programId = glCreateProgram();

  glAttachShader(programId, vertexShader);
  glAttachShader(programId, fragmentShader);
  glLinkProgram(programId);

  glGetProgramiv(programId, GL_LINK_STATUS, &status);
  // since shaders are already compiled and linked to program we delete them
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
  if (status == GL_FALSE) {
    GLint maxLength = 0;
    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(programId, maxLength, &maxLength, &infoLog[0]);
    std::cerr << "[SHADER] Shader Linking Error" << "\n";
    std::cerr << infoLog.data() << "\n";
    glDeleteProgram(programId);
    return -1;
  }
  return programId;
}

void ShaderProgram::reload() {
  auto programId = init();
  if (programId == -1) {
    return;
  }
  glDeleteProgram(m_Handle);
  m_Handle = programId;
}
} // namespace Renderer
