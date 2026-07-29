#include "file.h"
#include "shader.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <filesystem>
#include <iostream>
#include <linux/input.h>
#include <vector>
namespace Renderer {

ShaderProgram::ShaderProgram(const std::filesystem::path &vertexPath,
                             const std::filesystem::path &fragmentPath)
    : m_vertexPath(vertexPath), m_fragmentPath(fragmentPath) {
  auto programId = init();
  if (programId != 0) {
    m_Handle = programId;
  }
}

GLuint ShaderProgram::init() {
  int status = 0;
  std::string fileData;
  const char *shaderSource;
  status = Core::ReadTextFile(m_vertexPath, fileData);
  if (status == 0) {
    std::cerr << "[SHADER] cannot open vertex shader file";
    std::cerr << "Failed to open '" << m_vertexPath << '\n';
    return -1;
  }
  shaderSource = fileData.c_str();
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &shaderSource, 0);

  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

  char infologtemp[512];
  if (status == GL_FALSE) {
    int infoLogLen;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLen);
    std::cerr << "[SHADER] Vertex shader compilation failed" << "\n";
    std::string infoLog(infoLogLen, '\0');
    glGetShaderInfoLog(vertexShader, 512, NULL, infologtemp);
    std::cerr << infologtemp << "\n";
    glDeleteShader(vertexShader);
    return -1;
  }

  status = Core::ReadTextFile(m_fragmentPath, fileData);
  if (status == 0) {
    std::cerr << "[SHADER] cannot open fragment shader file";
    std::cerr << "Failed to open '" << m_fragmentPath << '\n';
    return -1;
  }
  shaderSource = fileData.c_str();

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &shaderSource, 0);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    std::cerr << "[SHADER] Fragment shader compilation failed" << "\n";
    std::string infoLog;
    glGetShaderInfoLog(fragmentShader, 0, 0, &infoLog[0]);
    std::cerr << infoLog << "\n";
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

void ShaderProgram::use() const { glUseProgram(m_Handle); }

const GLuint ShaderProgram::getId() const { return m_Handle; }

} // namespace Renderer
