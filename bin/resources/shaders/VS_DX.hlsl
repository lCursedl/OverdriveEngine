//--------------------------------------------------------------------------------------
// File: Tutorial07_2.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
#define MAX_BONES 200
cbuffer ConstantBuffer : register( b0 )
{
	matrix World;
	matrix View;
	matrix Projection;
    float4 vMeshColor;
};

cbuffer BoneCB : register(b1)
{
	matrix gBones[MAX_BONES];
};
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Pos 		 : POSITION;
	float2 Tex 		 : TEXCOORD0;
	float3 Normal 	 : NORMAL0;
	float3 Tangent  : TANGENT0;
	float3 Binormal : BINORMAL0;
	int4   BoneID   : BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};
//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	matrix BoneTransform = mul(gBones[input.BoneID.x], input.Weights.x);
	BoneTransform		+= mul(gBones[input.BoneID.y], input.Weights.y);
	BoneTransform		+= mul(gBones[input.BoneID.z], input.Weights.z);
	BoneTransform		+= mul(gBones[input.BoneID.w], input.Weights.w);

	output.Pos = mul(float4(input.Pos, 1.0), BoneTransform);
    //output.Pos = mul(float4(input.Pos, 1.0f), World );
	output.Pos = mul( output.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Tex = input.Tex;
    
    return output;
}