#version 460 core
out vec4 FragColor;
in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

#define DIRECTIONAL 1
#define POINT 2
#define SPOT 3

struct Material {
  vec4 fillColor;
  bool hasNormalMap;
  sampler2D specular;
  sampler2D diffuse;
  sampler2D normal;
  float shininess;
};

struct Light {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  vec3 pos;
  vec3 dir;
  float cutoff;
  float outerCutoff;

  float constant;
  float linear;
  float quadratic;

  uint type;
};

uniform Material material;
uniform Light[10] lights;
uniform vec3 CameraPosition;

vec4 calculateDiffuse(in vec3 lightToFragmentDir, in vec4 diffuseColor, in vec3 lightDiffuse) {
  float diffuseAmt = max(abs(dot(-lightToFragmentDir, Normal)), 0.0);
  return diffuseAmt * diffuseColor * vec4(lightDiffuse, 1.0);
}

vec4 calculateSpecular(in vec3 lightToFragmentDir, in vec4 specularColor, in vec3 lightSpecular) {
  vec3 reflectionDir = normalize(reflect(-lightToFragmentDir, Normal));
  vec3 cameraDir = normalize(CameraPosition - Position);
  float specularAmt = pow(max(dot(reflectionDir, cameraDir), 0.0), material.shininess);
  return specularAmt * specularColor * vec4(lightSpecular, 1.0);
}

vec4 calculateDirectionalLight(in Light l, in vec4 diffuseColor, in vec4 specularColor) {
  vec4 ambient = vec4(l.ambient, 1.0) * diffuseColor;
  vec4 diffuse = calculateDiffuse(l.dir, diffuseColor, l.diffuse);
  vec4 specular = calculateSpecular(l.dir, specularColor, l.specular);
  return ambient + diffuse + specular;
}

vec4 calculatePointLight(in Light l, in vec4 diffuseColor, in vec4 specularColor) {
  vec3 lightToFragmentDir = Position - l.pos;
  float distance = length(lightToFragmentDir);
  float attenuation = 1.0 / (l.constant + l.linear * distance + l.quadratic * distance * distance);
  lightToFragmentDir = normalize(lightToFragmentDir);

  vec4 ambient = vec4(l.ambient, 1.0) * diffuseColor * attenuation;
  vec4 diffuse = calculateDiffuse(lightToFragmentDir, diffuseColor, l.diffuse) * attenuation;
  vec4 specular = calculateSpecular(lightToFragmentDir, specularColor, l.specular) * attenuation;
  return ambient + diffuse + specular;
}

vec4 calculateSpotLight(in Light l, in vec4 diffuseColor, in vec4 specularColor) {
  vec3 lightToFragmentDir = Position - l.pos;
  float distance = length(lightToFragmentDir);
  lightToFragmentDir = normalize(lightToFragmentDir);
  float cosineAngle = dot(normalize(l.dir), lightToFragmentDir);

  float attenuation = 1.0 / (l.constant + l.linear * distance + l.quadratic * distance * distance);
  vec4 ambient = vec4(l.ambient, 1.0) * diffuseColor * attenuation;
  // meaning the fragment is outside outer cutoff
  if (cosineAngle < l.outerCutoff) {
    return ambient;
  }

  vec4 diffuse = calculateDiffuse(lightToFragmentDir, diffuseColor, l.diffuse) * attenuation;
  vec4 specular = calculateSpecular(lightToFragmentDir, specularColor, l.specular) * attenuation;

  // between outer and inner cutoff
  if (cosineAngle < l.cutoff)
  {
    return (ambient + diffuse + specular) * attenuation;
  }
  float fallOff = (cosineAngle - l.outerCutoff) / (l.cutoff - l.outerCutoff);
  return (ambient + diffuse + specular) * attenuation * fallOff;
}

void main() {
  vec4 diffuseColor = texture(material.diffuse, TexCoord) * material.fillColor;
  vec4 specularColor = texture(material.specular, TexCoord);
  vec4 accumulatedColor = vec4(0.0);
  for (int i = 0; i < 10; i++) {
    Light l = lights[i];
    switch (l.type) {
      case DIRECTIONAL:
      accumulatedColor += calculateDirectionalLight(l, diffuseColor, specularColor);
      break;
      case POINT:
      accumulatedColor += calculatePointLight(l, diffuseColor, specularColor);
      break;
      case SPOT:
      accumulatedColor += calculateSpotLight(l, diffuseColor, specularColor);
    }
  }
  FragColor = accumulatedColor;
}
