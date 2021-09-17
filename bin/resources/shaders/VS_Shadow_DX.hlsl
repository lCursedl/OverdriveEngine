cbuffer WVP : register (b0) {
	matrix View;
	matrix Projection;
	float4 ObjectPos;
};

struct VS_INPUT {
	float3 msPos		: POSITION0;
	float2 texCoord		: TEXCOORD0;
	float3 normals		: NORMAL0;
	float3 tangents		: TANGENT0;
	float3 binormals	: BINORMAL0;
};

struct VS_OUTPUT {
	float4 psPos		: POSITION;
	float depthPos      : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT Input) {
	VS_OUTPUT Output;
	
	matrix Worldvs;
	Worldvs[0] = float4(1, 0, 0, 0);
    Worldvs[1] = float4(0, 1, 0, 0);
    Worldvs[2] = float4(0, 0, 1, 0);
    Worldvs[3] = ObjectPos;
	
	matrix matWV = mul(Worldvs, View);
	matrix matClip = mul(matWV, Projection);
	Output.psPos = mul(float4(Input.msPos, 1.0f), matClip);
	Output.depthPos = mul(float4(Input.msPos, 1.0f), matWV).z;
	
	return Output;
}