#version 450 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D Image;
uniform vec3 SpriteColor;

void main()
{    
    FragColor = vec4(SpriteColor,1.0) * texture(Image, TexCoords);
}  