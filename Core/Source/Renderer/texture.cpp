#include "texture.h"
#include "embededimages.h"
#include "file.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
namespace Renderer {

static std::unordered_map<BuiltInTexture, std::shared_ptr<Texture>>
    builtintextures;

static std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

std::shared_ptr<Texture> TextureFromMemory(const unsigned char *data, int width,
                                           int height, int channels,
                                           int internalChannels) {

  std::shared_ptr<Texture> result = std::make_shared<Texture>();

  glCreateTextures(GL_TEXTURE_2D, 1, &(result->handle));

  glTextureParameteri(result->handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(result->handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(result->handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(result->handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  GLenum format = channels == 1   ? GL_RED
                  : channels == 3 ? GL_RGB
                  : channels == 4 ? GL_RGBA
                                  : GL_RED;

  GLenum internalFormat = channels == 1   ? GL_R8
                          : channels == 3 ? GL_RGB8
                          : channels == 4 ? GL_RGBA8
                                          : GL_R8;
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTextureStorage2D(result->handle, 1, internalFormat, width, height);
  glTextureSubImage2D(result->handle, 0, 0, 0, width, height, format,
                      GL_UNSIGNED_BYTE, data);
  glGenerateTextureMipmap(result->handle);
  return result;
}

std::shared_ptr<Texture> TextureFromImage(const Core::Image &i) {

  return TextureFromMemory(i.data, i.channels, i.height, i.width, i.channels);
}

std::shared_ptr<Texture> TextureFromFile(const std::filesystem::path &path) {
  auto key = std::filesystem::absolute(path).generic_string();
  if (textures.find(key) == textures.end()) {

    std::optional<Core::Image> i = Core::ReadImage(path);

    if (!i) {
      std::cerr << "[TEXTURE] Failed opening image: " << path << "\n";
      return GetBuiltInTexture(BuiltInTexture::ERROR);
    }

    std::cerr << "New texture created of key " << key << "\n";

    std::shared_ptr<Texture> result = TextureFromImage(i.value());
    result->path = path;
    textures[key] = result;
  }
  return textures[key];
}

static int count = 1;
// why is length int and not uint? I dont know
std::shared_ptr<Texture> TextureFromImageInMemory(unsigned char *data,
                                                  int length) {
  auto image = Core::ReadImageFromMemory(data, length);
  if (!image) {
    std::cerr << "[TEXTURE] Reading from memory but data doesnt exist";
    exit(-1);
  }
  // Core::WriteTestDefault("image" + std::to_string(count) + ".png",
  //                        image.value());

  count++;
  return TextureFromImage(image.value());
}

std::shared_ptr<Texture> GetBuiltInTexture(BuiltInTexture tex) {

  if (builtintextures.find(tex) == builtintextures.end()) {
    EmbeddedImage e;
    switch (tex) {
    case BuiltInTexture::DEFAULT:
      e = embeddedImagesMap[EmbeddedImages::DEFAULT];
      break;
    case BuiltInTexture::BLACK:
      e = embeddedImagesMap[EmbeddedImages::BLACK];
      break;
    case BuiltInTexture::ERROR:
      e = embeddedImagesMap[EmbeddedImages::ERROR];
      break;
    case BuiltInTexture::WHITE:
      e = embeddedImagesMap[EmbeddedImages::WHITE];
      break;
    }
    builtintextures[tex] = TextureFromImageInMemory(e.data, e.length);
  }
  return builtintextures[tex];
}

// std::shared_ptr<Texture> AtlasTexture(const std::string &path) {}

} // namespace Renderer
