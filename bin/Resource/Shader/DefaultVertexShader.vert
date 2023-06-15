#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 Normal;
out vec2 TexCoord; 
out vec3 FragPos;

void main()
{
	TexCoord = aTexCoord;
	FragPos = vec3(Model * vec4(aPos, 1.0));

	mat3 normalMatrix = transpose(inverse(mat3(Model)));

	Normal = normalize(normalMatrix * aNormal);;
	gl_Position = Projection * View * Model * vec4(aPos, 1.0);
}
