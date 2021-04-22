#version 420 core
out vec4 FragColor;

layout(std140, binding = 0) uniform Matrices
{
	mat4 World;
	mat4 View;
	mat4 Projection;
	vec4 Color;
};

in vec2 Texcoord;
in vec4 psPos;

void main()
{
	FragColor = psPos;
}