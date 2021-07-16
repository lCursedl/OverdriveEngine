#version 420 core
out vec4 FragColor;

const int MAX_BONES = 200;

layout(std140, binding = 0) uniform Matrices {
	mat4 World;
	mat4 View;
	mat4 Projection;
	vec4 Color;
};

layout(std140, binding = 1) uniform Bones
{
	mat4 gBones[MAX_BONES];
};

in vec2 Texcoord;

layout (binding = 0) uniform sampler2D diffuse;

void main() {
	FragColor = texture(diffuse, Texcoord);
}