#include "file.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include "Renderer/embededimages.h"
#include "stb_image.h"
#include "stb_image_write.h"
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
  stbi_set_flip_vertically_on_load_thread(true);
  Image result;
  result.data = stbi_load(path.c_str(), &result.width, &result.height,
                          &result.channels, 0);
  if (!result.data) {
    return std::nullopt;
  }
  result.imagePath = path;

  return result;
}

std::optional<Image> ReadImageFromMemory(unsigned char *buffer, int length) {
  stbi_set_flip_vertically_on_load(true);
  Image result;
  result.data = stbi_load_from_memory(buffer, length, &result.width,
                                      &result.height, &result.channels, 0);
  if (!result.data) {
    return std::nullopt;
  }

  return result;
}

/**
 * @brief Creates a new png image of given name with specified name. This
 * function only takes contigious arraf for buffer and stride is used as
 * width*components
 *
 * @param name Name of Image
 * @param width Width of Image
 * @param height Height of Image
 * @param components Components of Image ie (1 for Monochrome, 2 for alpha
 * monochrome, 3 for rgb, 4 for rgba, I don't know what happens if i pass more
 * than 4)
 * @param buffer
 * @param length
 * @return returns true if success false if not success
 */

bool WritePng(std::string name, int width, int height, int components,
              unsigned char *buffer) {
  return stbi_write_png(name.c_str(), width, height, components, buffer,
                        width * components);
}

void WriteTestDefault(const std::string name, const Image &im) {
  auto defaultIm = &im;
  stbi_flip_vertically_on_write(1);
  stbi_write_png(name.c_str(), defaultIm->width, defaultIm->height,
                 defaultIm->channels, defaultIm->data,
                 defaultIm->width * defaultIm->channels);
}

}; // namespace Core
