#pragma once
#include "glad/glad.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <filesystem>
namespace Renderer {

enum class EShaders { BasicShader = 1 };

class ShaderProgram {

public:
  ShaderProgram(const std::filesystem::path &vertexPath,
                const std::filesystem::path &fragmentPath);
  void reload();

  void use() const;

  const GLuint getId() const;

  void SetMat4(const std::string &name, const glm::mat4 &mat) const;
  void SetInt(const std::string &name, const GLint val) const;
  void SetFloat(const std::string &name, GLfloat val) const;
  void SetVec4(const std::string &name, const glm::vec4 &val) const;
  void SetVec3(const std::string &name, const glm::vec3 &val) const;

private:
  GLuint m_Handle;
  std::filesystem::path m_vertexPath;
  std::filesystem::path m_fragmentPath;

  GLuint init();
};

std::shared_ptr<ShaderProgram> getShader(EShaders es);
} // namespace Renderer
