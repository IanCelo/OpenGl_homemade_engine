#version 450 core

layout (location = 0) in vec4 Vertex; // <vec2 position, vec2 texCoords>
out vec2 TexCoords;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    TexCoords = Vertex.zw;
    gl_Position = Projection * View * Model * vec4(Vertex.xy, 0.0, 1.0);
}