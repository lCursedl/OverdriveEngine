#version 420 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexcoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBinormal;

layout(std140, binding = 0) uniform Matrices
{
	mat4 World;
	mat4 View;
	mat4 Projection;
	vec4 Color;
};

out vec2 Texcoord;
out vec4 psPos;

void main()
{
	psPos = World * vec4(aPosition.xyz, 1.0);
	gl_Position = Projection * View * psPos;
	Texcoord = aTexcoord;
}