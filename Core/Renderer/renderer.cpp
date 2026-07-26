#include "renderer.h"
namespace Renderer {
Texture CreateTexture(int width, int height) {
  Texture result;
  result.Width = width;
  result.Height = height;

  glGenTextures(1, &result.Handle);
  return result;
}
} // namespace Renderer
