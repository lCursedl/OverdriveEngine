//Constant buffer

cbuffer WVP : register (b0) {
	matrix World;
	matrix View;
	matrix Projection;
};
//Input layout

struct VS_INPUT {
	float3 msPos		: POSITION0;
	float2 texCoord	: TEXCOORD0;
	float3 normals		: NORMAL0;
	float3 tangents	: TANGENT0;
	float3 binormals	: BINORMAL0;
};
//Output structure

struct VS_OUTPUT {
	float4 psPos		: SV_POSITION;
	float2 texCoord	: TEXCOORD0;
	float3 posView		: TEXCOORD1;
	float3x3 matrixTBN	: TEXCOORD2;
};

VS_OUTPUT main(VS_INPUT Input) {
	VS_OUTPUT Output;
	matrix matWV = mul(World, View);
	
	//View space pos
	Output.posView = mul(float4(Input.msPos.xyz, 1.0f), matWV).xyz;
	//Proyection space pos
	Output.psPos = mul(float4(Output.posView, 1.0f), Projection);
	//World space normal
	float3 wsNormal = normalize(mul(float4(Input.normals.xyz, 0.0f), matWV).xyz);
	//World space binormal
	float3 wsBinormal = normalize(mul(float4(Input.binormals.xyz, 0.0f), matWV).xyz);
	//World space tangent
	float3 wsTangent = normalize(mul(float4(Input.tangents.xyz, 0.0f), matWV).xyz);
	
	Output.texCoord = Input.texCoord;
	Output.matrixTBN = float3x3(wsTangent.xyz, wsBinormal.xyz, wsNormal.xyz);
	
	return Output;
}