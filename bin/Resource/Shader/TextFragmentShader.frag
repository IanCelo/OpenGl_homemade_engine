#version 450 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D Text;
uniform vec3 TextColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(Text, TexCoords).r);
    FragColor = vec4(TextColor, 1.0) * sampled;
}  