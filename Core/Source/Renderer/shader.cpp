#include "glad/glad.h"
#include "file.h"
#include "shader.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "glm/ext.hpp"
#include "glm/ext/vector_float4.hpp"
#include "glm/gtc/type_ptr.hpp"
namespace Renderer {

static std::shared_ptr<ShaderProgram> s_PhongShader;
static std::shared_ptr<ShaderProgram> s_TextShader;

std::shared_ptr<ShaderProgram> getShader(EShaders es) {
  switch (es) {
  case EShaders::PhongShader:
    if (!s_PhongShader) {
      s_PhongShader =
          std::make_shared<ShaderProgram>("../assets/shaders/phongShader.vert",
                                          "../assets/shaders/phongShader.frag");
    }
    return s_PhongShader;
    break;
  case EShaders::TEXTSHADER:
    if (!s_TextShader) {
      s_TextShader =
          std::make_shared<ShaderProgram>("../assets/shaders/textShader.vert",
                                          "../assets/shaders/textShader.frag");
    }
    return s_TextShader;
  default:
    std::cerr << "[getShader] No Shader of that type";
    return nullptr;
    break;
  }
}

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
  const char *shaderSource;
  auto fileData = Core::ReadTextFile(m_vertexPath);
  if (!fileData) {
    std::cerr << "[SHADER] cannot open vertex shader file" << "\n";
    std::cerr << "Failed to open '" << m_vertexPath << '\n';
    return -1;
  }
  shaderSource = fileData->c_str();
  auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &shaderSource, 0);

  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

  if (status == GL_FALSE) {
    int infoLogLen;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLen);
    std::cerr << "[SHADER] Vertex shader compilation failed"
              << "\n";
    std::string infoLog(infoLogLen, '\0');
    glGetShaderInfoLog(vertexShader, infoLogLen, NULL, infoLog.data());
    std::cerr << infoLog << "\n";
    glDeleteShader(vertexShader);
    return -1;
  }

  fileData = Core::ReadTextFile(m_fragmentPath);
  if (!fileData) {
    std::cerr << "[SHADER] cannot open fragment shader file" << "\n";
    std::cerr << "Failed to open '" << m_fragmentPath << '\n';
    return -1;
  }
  shaderSource = fileData->c_str();

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &shaderSource, 0);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    std::cerr << "[SHADER] Fragment shader compilation failed" << "\n";
    int infoLogLen;
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLen);
    std::string infoLog(infoLogLen, '\0');
    glGetShaderInfoLog(fragmentShader, infoLogLen, NULL, infoLog.data());
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

void ShaderProgram::SetMat4(const std::string &name,
                            const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(m_Handle, name.c_str()), 1, false,
                     glm::value_ptr(mat));
}

void ShaderProgram::SetInt(const std::string &name, GLint val) const {
  glUniform1i(glGetUniformLocation(m_Handle, name.c_str()), val);
}

void ShaderProgram::SetUInt(const std::string &name, GLuint val) const {
  glUniform1ui(glGetUniformLocation(m_Handle, name.c_str()), val);
}

void ShaderProgram::SetFloat(const std::string &name, GLfloat val) const {
  glUniform1f(glGetUniformLocation(m_Handle, name.c_str()), val);
}

void ShaderProgram::SetVec4(const std::string &name,
                            const glm::vec4 &val) const {
  glad_glUniform4fv(glGetUniformLocation(m_Handle, name.c_str()), 1,
                    glm::value_ptr(val));
}

void ShaderProgram::SetVec3(const std::string &name,
                            const glm::vec3 &val) const {
  glad_glUniform3fv(glGetUniformLocation(m_Handle, name.c_str()), 1,
                    glm::value_ptr(val));
}

} // namespace Renderer
