#pragma once
#include "file.h"
#include "glad/glad.h"
#include <filesystem>
#include <memory>
#include <string>
namespace Renderer {

struct Texture {
  GLuint handle;
  std::string path;
};

enum class BuiltInTexture { DEFAULT = 1, BLACK, ERROR };
enum class TextureType { DIFFUSE = 1, SPECULAR };

std::shared_ptr<Texture> TextureFromFile(const std::filesystem::path &path);
std::shared_ptr<Texture> TextureFromMemory(unsigned char *data, int length);
std::shared_ptr<Texture> TextureFromImage(const Core::Image &i);
std::shared_ptr<Texture> GetBuiltInTexture(BuiltInTexture tex);

} // namespace Renderer
