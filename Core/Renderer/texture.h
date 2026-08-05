#pragma once
#include "glad/glad.h"
#include <filesystem>
#include <memory>
#include <string>
namespace Renderer {
struct Texture {
  GLuint handle;
  std::string path;
  std::string type;
};

std::shared_ptr<Texture> TextureFromFile(const std::filesystem::path &path,
                                         const std::string &type);

} // namespace Renderer
