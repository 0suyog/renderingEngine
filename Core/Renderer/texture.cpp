#include "texture.h"
#include "file.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>
namespace Renderer {

static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

std::shared_ptr<Texture> TextureFromFile(const std::filesystem::path &path,
                                         const std::string &type) {
  auto key = std::filesystem::absolute(path).generic_string() + type;
  if (textures[key] == nullptr) {

    std::cerr << "New texture created of key " << key << "\n";

    std::shared_ptr<Texture> result = std::make_shared<Texture>();
    std::optional<Core::Image> i = Core::ReadImage(path);

    if (!i) {
      std::cerr << "[TEXTURE] Failed opening image: " << path << "\n";
      return nullptr;
    }
    glCreateTextures(GL_TEXTURE_2D, 1, &(result->handle));

    glTextureParameteri(result->handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(result->handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(result->handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(result->handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum format = i->channels == 1   ? GL_RED
                    : i->channels == 3 ? GL_RGB
                    : i->channels == 4 ? GL_RGBA
                                       : GL_RED;

    GLenum internalFormat = i->channels == 1   ? GL_R8
                            : i->channels == 3 ? GL_RGB8
                            : i->channels == 4 ? GL_RGBA8
                                               : GL_R8;

    glTextureStorage2D(result->handle, 1, internalFormat, i->width, i->height);
    glTextureSubImage2D(result->handle, 0, 0, 0, i->width, i->height, format,
                        GL_UNSIGNED_BYTE, i->data);
    glGenerateTextureMipmap(result->handle);
    result->path = path;
    result->type = type;
    textures[key] = result;
  } else {
    std::cerr << "Loading texture from cache key: " << key << "\n";
  }
  return textures[key];
}
} // namespace Renderer
