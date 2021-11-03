//Render target
RWTexture2D<float4> ssaoTex : register(u0);
//Texture objects
Texture2D positionTex : register(t0);
Texture2D normalTex : register(t1);
//Sampler objects
SamplerState lSampler : register(s0);
//Constant buffer
cbuffer SSAO : register (b0) {
	float kIntensity;
	float kScale;
	float kBias;
	float kSample;
};
/*
uint3 groupID : SV_GroupID
- Index of the group within the dispatch for each dimension

uint3 groupThreadID : SV_GroupThreadID
- Index of the thread within the group for each dimension

uint groupIndex : SV_GroupIndex
- A sequential index within the group that starts from 0 top left back and goes on to bottom right front

uint3 dispatchThreadID : SV_DispatchThreadID 
- Global thread index within the whole dispatch, write and read a buffer
*/
float3 GetPosition(float2 uv) {
	return positionTex.SampleLevel(lSampler, uv, 0).xyz;	 
}

float3 GetNormal(float2 uv) {
	return normalTex.SampleLevel(lSampler, uv, 0).xyz;
}

float3 GetRandom(float2 uv) {
	float noiseX = frac(sin(dot(uv, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f);
	float noiseY = frac(sin(dot(uv, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f);
	float noiseZ = frac(sin(dot(uv, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f);
	
	return normalize(float3(noiseX, noiseY, noiseZ));
}

float DoAmbientOcclusion(float2 texCoord, float2 uv, float3 pos, float3 normal)
{
  float3 diff = GetPosition(texCoord + uv) - pos;
  const float3 v = normalize(diff);
  const float d = length(diff) * kScale;
  return max(0.0f, dot(normal, v) - kBias) * (1.0 / (1.0 + d)) * kIntensity;
}
[numthreads(32, 32, 1)]
void
main(uint3 groupThreadID    : SV_GroupThreadID,
	 uint3 groupID           : SV_GroupID,
	 uint3 dispatchThreadID : SV_DispatchThreadID,
	 uint  groupIndex        : SV_GroupIndex) {
	 
	if(dispatchThreadID.x > 800 || dispatchThreadID.y > 600) {
		return;
	}
	
	uint2 indexColor = dispatchThreadID.xy;
	
	float2 index = dispatchThreadID.xy / float2(800, 600);
	
	ssaoTex[indexColor] = float4(1.0f, 1.0f, 1.0f, 1.0f);
	
	// sampler position
	const float2 vec[4] =
	{
		float2(1,0), float2(-1,0), float2(0,1), float2(0,-1)
	};
	
	// current position
	float3 wsPos = GetPosition(index);
	
	// current normal
	float3 wsNormal = GetNormal(index);
	
	// current random
	float3 rand = GetRandom(index);
	
	// output AO
	float ao = 0.0f;
	
	// sampler radius
	float rad = kSample / wsPos.z;
	
	// sampler addition 
	int iterations = 4;
	for (int i = 0; i < iterations; ++i)
	{
		float2 coord1 = reflect(vec[i], rand) * rad;
		float2 coord2 = float2(coord1.x * 0.707f - coord1.y * 0.707f,
							coord1.x * 0.707f + coord1.y * 0.707f);
	
		ao += DoAmbientOcclusion(index, coord1 * 0.25f, wsPos.xyz, wsNormal.xyz);
		ao += DoAmbientOcclusion(index, coord2 * 0.5f, wsPos.xyz, wsNormal.xyz);
		ao += DoAmbientOcclusion(index, coord1 * 0.75f, wsPos.xyz, wsNormal.xyz);
		ao += DoAmbientOcclusion(index, coord2, wsPos.xyz, wsNormal.xyz);
	}
	ao /= (iterations * 4);
	
	ssaoTex[indexColor] = float4(1.0f - ao.xxx, 1.0f);
}