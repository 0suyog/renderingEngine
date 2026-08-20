#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include <filesystem>
namespace Renderer {
class ShaderProgram {

public:
  ShaderProgram(const std::filesystem::path &vertexPath,
                const std::filesystem::path &fragmentPath);
  void reload();

  void use() const;

  const GLuint getId() const;

  void SetMat4(const std::string &name, const glm::mat4 &mat) const;
  void SetInt(const std::string &name, const GLint val) const;

private:
  GLuint m_Handle;
  std::filesystem::path m_vertexPath;
  std::filesystem::path m_fragmentPath;

  GLuint init();
};
} // namespace Renderer
