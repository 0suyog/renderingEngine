#include "light.h"
#include "glm/ext/vector_float3.hpp"
namespace Renderer {

Light DirectionalLight(glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular,
                       glm::vec3 direction, glm::vec3 position) {
  return Light{.ambient = ambient,
               .diffuse = diffuse,
               .specular = specular,
               .pos = position,
               .dir = direction,
               .type = LIGHTTYPE::DIRECTIONAL};
}

Light DirectionalLight(glm::vec3 color, glm::vec3 direction,
                       glm::vec3 position) {
  return DirectionalLight(color, color, color, direction, position);
}

Light PointLight(glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular,
                 float quadratic, float linear, float constant,
                 glm::vec3 position) {
  return Light{.ambient = ambient,
               .diffuse = diffuse,
               .specular = specular,
               .pos = position,
               .constant = constant,
               .linear = linear,
               .quadratic = quadratic,
               .type = LIGHTTYPE::POINT};
}

Light PointLight(glm::vec3 color, float quadratic, float linear, float constant,
                 glm::vec3 position) {
  return PointLight(color, color, color, quadratic, linear, constant, position);
}

Light SpotLight(glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular,
                float cutoff, float outerCutoff, float quadratic, float linear,
                float constant, glm::vec3 position) {
  return Light{.ambient = ambient,
               .diffuse = diffuse,
               .specular = specular,
               .pos = position,
               .cutoff = cutoff,
               .outerCutoff = outerCutoff,
               .constant = constant,
               .linear = linear,
               .quadratic = quadratic,
               .type = LIGHTTYPE::SPOT};
}

Light SpotLight(glm::vec3 color, float cutoff, float outerCutoff,
                float quadratic, float linear, float constant,
                glm::vec3 position) {
  return SpotLight(color, color, color, cutoff, outerCutoff, quadratic, linear,
                   constant, position);
}

} // namespace Renderer
