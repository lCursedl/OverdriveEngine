//Texture objects
Texture2D albedoMap : register (t0);
Texture2D normalMap : register (t1);
Texture2D metalMap : register  (t2);
Texture2D roughMap : register  (t3);

//Sampler objects
SamplerState linearSampler : register (s0);

//Input structure
struct PS_INPUT {
	float4 psPos		: SV_POSITION;
	float2 texCoord	: TEXCOORD0;
	float3 posView		: TEXCOORD1;
	float3x3 matrixTBN	: TEXCOORD2;
};

//Output structure
struct PS_OUTPUT {
	float4 pos 	: SV_TARGET0;
	float4 normal  : SV_TARGET1;
	float4 albedo  : SV_TARGET2;
};

PS_OUTPUT main(PS_INPUT Input) : SV_TARGET {
	PS_OUTPUT Output = (PS_OUTPUT)0;
	
	//Position
	Output.pos = float4(Input.posView.xyz, 1.0f);
	
	//Normal
	float3 normal = normalMap.Sample(linearSampler, Input.texCoord).xyz;
	normal = (normal * 2.0f) - 1.0f;
	normal = normalize(mul(normal, Input.matrixTBN).xyz);
	Output.normal.xyz = normal;
	//Roughness
	Output.normal.w = roughMap.Sample(linearSampler, Input.texCoord).x;
	
	//Albedo
	float3 albedo = albedoMap.Sample(linearSampler, Input.texCoord).xyz;
	Output.albedo.xyz = albedo;
	//Metallic
	Output.albedo.w = metalMap.Sample(linearSampler, Input.texCoord).x;
	
	return Output;
}