#version 460 core
uniform sampler2D atlas;

in vec2 TexCoord;
out vec4 FragColor;

uniform vec4 bgColor;
uniform vec4 fgColor;

float median(float r, float g, float b) {
  return max(min(r, g), min(max(r, g), b));
}

float pxRange = 2.0; // set to distance field's pixel range

vec2 sqr(vec2 x) {
  return x * x;
} // squares vector components

float screenPxRange() {
  vec2 unitRange = vec2(pxRange) / vec2(textureSize(atlas, 0));
  // If inversesqrt is not available, use vec2(1.0)/sqrt
  vec2 screenTexSize = inversesqrt(sqr(dFdx(TexCoord)) + sqr(dFdy(TexCoord)));
  // Can also be approximated as screenTexSize = vec2(1.0)/fwidth(texCoord);
  return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main() {
  vec3 msd = texture(atlas, TexCoord).rgb;
  float sd = median(msd.r, msd.g, msd.b);
  float screenPxDistance = screenPxRange() * (sd - 0.5);
  float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
  FragColor = mix(bgColor, fgColor, opacity);
  // FragColor = vec4(0, 1, 0, 1);
}
