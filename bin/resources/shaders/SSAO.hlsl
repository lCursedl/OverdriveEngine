Texture2D positionTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState sampler : register(s0);

float2 viewportDimensions;

float g_sampleRadius;
float g_intensity;
float g_scale;
float g_bias;

struct PS_INPUT {
  float2 TexCoord : TEXCOORD0;
}

struct PS_OUTPUT {
  float4 Color : COLOR0;
}

float4 GetPosition(in float2 uv) {
  return positionTexture.Sample(sampler, uv);
}

float3 GetNormal(in float2 uv) {
  return normalize(normalTexture.sample(sampler, uv));
}

float2 GetRandom(in float2 uv) {
  float noiseX = (frac(sin(dot(uv, float2(15.8989f, 76.132f) * 1.0f)) * 46336.23745f));
  float noiseY = (frac(sin(dot(uv, float2(11.9899f, 62.223f) * 2.0f)) * 34748.34744f));
  float noiseZ = (frac(sin(dot(uv, float2(13.3238f, 63.122f) * 3.0f)) * 59998.47362f));
}

float DoAmbientOcclusion(in float texCoord, in float2 uv, in float3 p, in float3 cNorm) {
  float3 diff = GetPosition(texCoord + uv) - p;
  const float3 v = normalize(diff);
  const float d = length(diff) * g_scale;
  return max(0.0f, dot(cNorm, v) - g_bias) * (1.0f / (1.0f + d)) * g_intensity;
}

PS_OUTPUT main(PS_INPUT input) {
  PS_OUTPUT Output;
  Output.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
  
  const float2 vec[4] = {
    float2(1, 0), float2(-1, 0), float2(0, 1), float2(0, -1),
  };
  
  float4 p = GetPosition(input.TexCoord);
  float3 n = GetNormal(input.TexCoord);
  float2 rand = GetRandom(input.TexCoord);
  
  float ao = 0.0f;
  float rad = g_sampleRadius / p.z;
  
  int iterations = 4;
  for (int j = 0; j < iterations; ++j) {
    float2 coord1 = reflect(vec[j], rand) * rad;
	float2 coord2 = float2(coord1.x * 0.707f - coord.y * 0.707f);
	
	ao += DoAmbientOcclusion(input.TexCoord, coord1 * 0.25f, p.xyz, n);
	ao += DoAmbientOcclusion(input.TexCoord, coord2 * 0.5f , p.xyz, n);
	ao += DoAmbientOcclusion(input.TexCoord, coord1 * 0.75f, p.xyz, n);
	ao += DoAmbientOcclusion(input.TexCoord, coord2,          p.xyz, n);
  }
  ao /= (iterations * 4);
  Output.Color.rgb = 1 - ao.xxx;
  return Output;
}