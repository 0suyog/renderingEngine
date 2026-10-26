#pragma once
#include "glad/glad.h"
#include "application.h"
#include "file.h"
#include <filesystem>
#include <iostream>

namespace Renderer {

class Font;

/**
 * @struct Texture
 * @brief Structure that stores path of image or other source used to create
 * texture (may not always be available) and texture id of OpenGl Texture
 *
 */

class Texture {
public:
  GLuint handle = 0;
};

enum class BuiltInTexture { DEFAULT = 1, BLACK, ERROR, WHITE };
enum class TextureType { DIFFUSE = 1, SPECULAR };

/**
 * @brief Creates OpenGl texture object from an image
 *
 * @param path: path of image
 */
std::shared_ptr<Texture> TextureFromFile(const std::filesystem::path &path);

/**
 * @brief if image already exist in memory then this uses that data to create a
 * texture no path is stored so path is empty for generated image
 *
 * @param data literal bytes of image
 * @param length length of data
 */
std::shared_ptr<Texture> TextureFromImageInMemory(unsigned char *data,
                                                  int length);
/**
 * @brief Crete a Texture from image already loaded in memory
 *
 * @param i Image to create texture from
 */
std::shared_ptr<Texture> TextureFromImage(const Core::Image &i);
/**
 * @brief Retrieve embedded textures that engine provides
 *
 * @param tex Enum value of BuiltInTexture to retrieve
 */
std::shared_ptr<Texture> GetBuiltInTexture(BuiltInTexture tex);

/**
 * @brief Create new texture from color bytes in memory
 *
 * @param data array containing color bytes in memory should be width * height *
 * channels long
 * @param width width of texture to be created
 * @param height height of texture to be created
 * @param channels channels the color bytes are in
 * @param internalChannels channels the texture that will be stored as
 */
std::shared_ptr<Texture> TextureFromMemory(const unsigned char *data, int width,
                                           int height, int channels,
                                           int internalChannels);

std::shared_ptr<Texture> AtlasTexture(const Font &font);

} // namespace Renderer
