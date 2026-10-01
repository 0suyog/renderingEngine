#pragma once
#include "glm/ext/vector_float3.hpp"

namespace Renderer {

enum class LIGHTTYPE : unsigned int { DIRECTIONAL = 1, POINT, SPOT };

struct Light {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  glm::vec3 pos;
  glm::vec3 dir;
  float cutoff = 0.0f;
  float outerCutoff = 0.0f;

  float constant = 0.0f;
  float linear = 0.0f;
  float quadratic = 0.0f;

  LIGHTTYPE type = LIGHTTYPE::DIRECTIONAL;

  Light UsingOrigin(glm::vec3 origin) {
    if (type == LIGHTTYPE::DIRECTIONAL)
      return *this;
    auto retVal = *this;
    retVal.pos += origin;
    return retVal;
  }
};

Light DirectionalLight(glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular,
                       glm::vec3 direction, glm::vec3 position);

Light DirectionalLight(glm::vec3 color, glm::vec3 direction,
                       glm::vec3 position = {0.0f, 0.0f, 0.0f});

Light PointLight(glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular,
                 float quadratic, float linear, float constant = 1.0,
                 glm::vec3 position = {0, 0, 0});

Light PointLight(glm::vec3 color, float quadratic, float linear,
                 float constant = 1.0, glm::vec3 position = {0, 0, 0});

Light SpotLight(glm::vec3 diffuse, glm::vec3 ambient, glm::vec3 specular,
                float cutoff, float outerCutoff, float quadratic, float linear,
                float constant = 1.0, glm::vec3 position = {0, 0, 0});

Light SpotLight(glm::vec3 color, float cutoff, float outerCutoff,
                float quadratic, float linear, float constant = 1.0,
                glm::vec3 position = {0, 0, 0});

}; // namespace Renderer
