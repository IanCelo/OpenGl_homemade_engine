#version 450 core

layout (location = 0) in vec4 aPos;
layout (location = 1)  in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 scale;

out vec3 Normal;
out vec2 TexCoord; 
out vec3 FragPos;

void main()
{
	vec3 CameraRight_worldspace = vec3(View[0][0], View[1][0], View[2][0]);
	vec3 CameraUp_worldspace    = vec3(View[0][1], View[1][1], View[2][1]);
	vec3 center = vec3(Model[0][3], Model[1][3], Model[2][3]);
	vec3 position = center + CameraRight_worldspace * aPos.x * 1 + CameraUp_worldspace * aPos.y * 1;

	gl_Position =  Projection * View * Model * vec4(position, 1.0);

	FragPos		= vec3(Model * vec4(position, 1.0));
	Normal		= mat3(transpose(inverse(Model))) * aNormal;

	TexCoord = vec2(aPos.z, aPos.w);
}
