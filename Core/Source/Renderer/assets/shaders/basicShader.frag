#version 460 core
out vec4 FragColor;
in vec2 TexCoord;

struct Material{
  vec4 fillColor;
  float specularStrength;
  bool hasNormalMap;
  sampler2D specular;
  sampler2D diffuse;
  sampler2D normal;
};

uniform Material material;

void main(){
  vec4 accumulatedColor = material.fillColor* texture(material.diffuse,TexCoord);
  FragColor = accumulatedColor;
}
