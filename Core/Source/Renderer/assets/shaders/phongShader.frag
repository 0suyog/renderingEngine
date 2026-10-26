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

// test text

uniform sampler2D msdf;
vec4 bgColor = vec4(0, 0, 0, 1);
vec4 fgColor = vec4(1, 1, 1, 1);

float median(float r, float g, float b) {
  return max(min(r, g), min(max(r, g), b));
}

float pxRange = 2.0; // set to distance field's pixel range

vec2 sqr(vec2 x) {
  return x * x;
} // squares vector components

float screenPxRange() {
  vec2 unitRange = vec2(pxRange) / vec2(textureSize(msdf, 0));
  // If inversesqrt is not available, use vec2(1.0)/sqrt
  vec2 screenTexSize = inversesqrt(sqr(dFdx(TexCoord)) + sqr(dFdy(TexCoord)));
  // Can also be approximated as screenTexSize = vec2(1.0)/fwidth(texCoord);
  return max(0.5 * dot(unitRange, screenTexSize), 1.0);
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

  vec3 msd = texture(material.diffuse, TexCoord).rgb;
  float sd = median(msd.r, msd.g, msd.b);
  float screenPxDistance = screenPxRange() * (sd - 0.5);
  float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
  FragColor = mix(bgColor, fgColor, opacity);
}
