#version 450 core

layout (location = 0) in vec4 Vertex;
out vec2 TexCoords;

void main(void) {
  gl_Position = vec4(Vertex.xy, 0.0, 1.0);
  TexCoords = Vertex.zw;
}