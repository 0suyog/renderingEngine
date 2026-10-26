#pragma once
#include "stb_image.h"
#include "stb_image_write.h"
#include <filesystem>
#include <optional>
#include <string>
namespace Core {

struct Image {
  int width = 0, height = 0;
  int channels = 1;
  unsigned char *data = nullptr;
  std::filesystem::path imagePath;

  ~Image() noexcept {
    if (data)
      stbi_image_free(data);
  }

  Image() = default;

  Image(Image &&other) {
    if (this != &other) {
      stbi_image_free(data);
      width = other.width;
      height = other.height;
      channels = other.channels;
      data = other.data;
      other.data = nullptr;
    }
  }

  Image(const Image &) = delete;
  Image &operator=(const Image &) = delete;

  Image &operator=(Image &&other) noexcept {
    if (this != &other) {
      stbi_image_free(data);
      width = other.width;
      height = other.height;
      channels = other.channels;
      data = other.data;
      imagePath = other.imagePath;
      other.data = nullptr;
    }
    return *this;
  };
};

std::optional<std::string> ReadTextFile(const std::filesystem::path &path);
std::optional<Image> ReadImage(const std::filesystem::path &path);
std::optional<Image> ReadImageFromMemory(unsigned char *buffer, int length);
bool WritePng(const std::filesystem::path &name, int width, int height,
              int components, const unsigned char *buffer);

void WriteTestDefault(const std::string name, const Image &im);
} // namespace Core
