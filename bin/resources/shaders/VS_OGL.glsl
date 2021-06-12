#version 420 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexcoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBinormal;
layout (location = 5) in ivec4 aBoneID;
layout (location = 6) in vec4 aWeights;

const int MAX_BONES = 200;

layout(std140, binding = 0) uniform Matrices
{
	mat4 World;
	mat4 View;
	mat4 Projection;
	vec4 Color;
};

layout(std140, binding = 1) uniform Bones
{
	mat4 gBones[MAX_BONES];
};

out vec2 Texcoord;
out vec4 psPos;

void main()
{
	mat4 BoneTransform = gBones[aBoneID[0]] * aWeights[0];
	BoneTransform += gBones[aBoneID[1]] * aWeights[1];
	BoneTransform += gBones[aBoneID[2]] * aWeights[2];
	BoneTransform += gBones[aBoneID[3]] * aWeights[3];
	
	psPos = BoneTransform * vec4(aPosition, 1.0);
	gl_Position = Projection * View * World * psPos;
	Texcoord = aTexcoord;
}