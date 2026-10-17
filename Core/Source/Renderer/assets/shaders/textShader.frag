#version 460 core
uniform specular2D msdfAtlas;

in vec2 TexCoord;

void main() {
  FragColor = vec4(texture(msdfAtlas, TexCoord));
}
