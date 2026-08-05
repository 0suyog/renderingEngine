#include "file.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include "stb_image.h"
namespace Core {
std::optional<std::string> ReadTextFile(const std::filesystem::path &path) {
  std::ifstream f(path);
  if (!f.is_open()) {
    return std::nullopt;
  }
  f.seekg(0, std::ios::end);
  int size = f.tellg();
  f.seekg(0);
  std::string data;
  data.resize(size);
  f.read(data.data(), size);
  f.close();
  return data;
}

std::optional<Image> ReadImage(const std::filesystem::path &path) {
  stbi_set_flip_vertically_on_load(true);
  Image result;
  result.data = stbi_load(path.c_str(), &result.width, &result.height,
                          &result.channels, 0);
  if (!result.data) {
    return std::nullopt;
  }

  return result;
}

}; // namespace Core
