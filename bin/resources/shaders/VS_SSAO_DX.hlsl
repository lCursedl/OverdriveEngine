//Input structure
struct VS_INPUT {
	float3 msPos : POSITION0;
	float2 texCoord : TEXCOORD0;	
	float3 normals		: NORMAL0;
	float3 tangents	: TANGENT0;
	float3 binormals	: BINORMAL0;
};

//Output structure
struct VS_OUTPUT {
	float4 psPos	: SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT Input) {
	VS_OUTPUT Output;
	Output.psPos = float4(Input.msPos, 1.0f);
	Output.texCoord = Input.texCoord;
	
	return Output;
}