#define M_PI 3.14159265359
#define EPSILON 0.00001
#define SHADOW_BIAS 0.1

//Constant buffers
cbuffer lightBuffer : register (b0) {
	float3 lightPos;
	float lightIntensity;
	float4 viewPos;
	matrix matWV;
};

cbuffer lightMat : register (b1) {
	matrix View;
	matrix Projection;
	float4 Pos;
};

cbuffer cameraMat : register(b2) {
	matrix InverseView;
};

//Texture objects
Texture2D positionTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D albedoTex : register(t2);
Texture2D ssaoTex : register(t3);
Texture2D shadowTex : register(t4);

//Sampler objects
SamplerState lSampler : register(s0);
SamplerState sSampler : register(s1);

//Input structure
struct PS_INPUT {
	float4 psPos : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

float3 fresnelSchlick(float3 F0, float cosTheta)
{
  return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

float ndf_GGX(float NdH, float roughness)
{
  float alpha = roughness * roughness;
  float alphaSqr = alpha * alpha;

  float denom = (NdH * NdH) * (alphaSqr - 1.0f) + 1.0f;
  return alphaSqr / (M_PI * denom * denom);
}

float ga_SchlickG1(float cosTheta, float k)
{
  return cosTheta / (cosTheta * (1.0f - k) + k);
}

float ga_SchlickGGX(float cosLi, float cosLo, float roughness)
{
  float r = roughness + 1.0f;
  //Epic suggest using this roughness remaping for analytic lights
  float k = (r * r) / 8.0f;
  return ga_SchlickG1(cosLi, k) * ga_SchlickG1(cosLo, k);
}

float4 main (PS_INPUT Input) : SV_Target {
	float gamma = 2.2f;
	//Position sampler
	float4 wsPos = float4(positionTex.Sample(lSampler, Input.texCoord).xyz, 1.0f);
	//Normal sampler
	float4 wsNormal = normalTex.Sample(lSampler, Input.texCoord);
	float roughness = wsNormal.w;
	wsNormal.w = 0.0f;
	//Albedo sampler
	float4 albedo = albedoTex.Sample(lSampler, Input.texCoord);
	albedo.xyz = pow(albedo.xyz, gamma).xyz;
	float metallic = albedo.w;
	albedo.w = 1.0f;
	//Ambient occlusion
	float ao = ssaoTex.Sample(lSampler, Input.texCoord).r;
	//Specular
	float3 specular_F0 = lerp(0.04f, albedo.xyz, metallic);
	
	//Light Direction
	float4 ViewLightDir = mul(float4(lightPos, 1.0f), matWV);
	float4 ViewViewDir = mul(viewPos, matWV);
	
	float3 lightDir = normalize(ViewLightDir.xyz - wsPos.xyz);
	float3 viewDir = normalize(ViewViewDir.xyz - wsPos.xyz);
	
	//Diffuse Light
	float NdL = max(0.0f, dot(wsNormal.xyz, lightDir));
	float NdV = max(0.001f, dot(wsNormal.xyz, viewDir));
	
	float3 H = normalize(viewDir + lightDir);
	
	float NdH = max(0.001f, dot(wsNormal, H));
	float HdL = saturate(dot(H, lightDir));
	float HdV = saturate(dot(H, viewDir));
	
	float3 Reflect = normalize(reflect(-viewDir, wsNormal));
	
	float D = ndf_GGX(NdH, roughness);
	float3 F = fresnelSchlick(specular_F0, HdL);
	float G = ga_SchlickGGX(NdL, NdV, roughness);
	
	float3 specular = (D * F * G) / max(EPSILON, (NdL * NdV * 4.0f));
	
	//Shadow
	float shadow = 0.0f;
	float4 shadowPos = mul(wsPos, InverseView);
	float4 shadowWPos = mul(float4(shadowPos.xyz, 1.0f), View);
	float4 shadowClipPos = mul(shadowWPos, Projection);
	shadowClipPos /= shadowClipPos.w;
	//float pixelDepth = shadowClipPos.z /= shadowClipPos.w;
	float3 shadowTexCoords = 0.5f + (shadowClipPos.xyz * 0.5f);
	//shadowTexCoords.y = 1.0f - shadowTexCoords.y;
	
	// if(shadowtexcoords.x < 0.0f
		// || shadowtexcoords.x > 1.0f
		// || shadowtexcoords.y < 0.0f
		// || shadowtexcoords.y > 1.0f) {
		// shadowtest = float4(0, 0, 0, 1);
	// }
	shadowTexCoords.y = 1 - shadowTexCoords.y;
	float shadowDepth = shadowTex.Sample(sSampler, shadowTexCoords.xy).x;
	
	float currentDepth = shadowTexCoords.z;
	//shadow = currentDepth + SHADOW_BIAS > shadowDepth ? 0.0f : 1.0f;
	
	float2 texelSize = 1.0f / 1024.0f;
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {
			float pcfDepth = shadowTex.Sample(sSampler, shadowTexCoords.xy + float2(x, y) * texelSize).r;
			shadow += currentDepth - SHADOW_BIAS > pcfDepth ? 0.0f : 1.0f;
		}
	}	
	shadow /= 9.0f;
	
	return float4(pow(
					( (1.0f - shadow) * (albedo.xyz * NdL * lightIntensity) +
					(specular))
					,1.0f / gamma), 1.0f);
}