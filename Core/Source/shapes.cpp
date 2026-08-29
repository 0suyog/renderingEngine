#include "shapes.h"
#include "Renderer/model.h"
namespace Shape {
Polygon::Polygon(const glm::vec3 &center, float radius, int sides,
                 const glm::vec3 &color = {0.38f, 0.6f, 0.89f})
    : Renderer::Model(generateMesh()), sides(sides), radius(radius),
      center(center) {}

} // namespace Shape
