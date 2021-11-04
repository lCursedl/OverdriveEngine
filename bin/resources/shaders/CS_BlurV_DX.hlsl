//Render target
RWTexture2D<float4> blurTex : register(u0);
//Texture objects
Texture2D InputImage : register(t0);
//Sampler objects
SamplerState samLinear : register(s0);
//Constant buffers
cbuffer cbBlur : register(b0) {
	float4 fViewportDimensions;
}

float gamma = 1.0f;

[numthreads(32, 32, 1)]
void main(uint3 groupThreadID    : SV_GroupThreadID,
		  uint3 groupID           : SV_GroupID,
		  uint3 dispatchThreadID : SV_DispatchThreadID,
	      uint  groupIndex        : SV_GroupIndex) {
	if(dispatchThreadID.x > 800 || dispatchThreadID.y > 600) {
		return;
	}
	uint2 indexColor = dispatchThreadID.xy;
	
	float2 index = dispatchThreadID.xy / float2(800, 600);
	
	float offset[3] = {0.0, 1.3846153846, 3.2307692308};
	float weight[3] = {0.2270270270, 0.316216162, 0.0702702703};
  
	float4 FragmentColor = pow(InputImage.SampleLevel(samLinear, index, 0), 1.0f) * weight[0];
  
	for(int i = 1; i < 3; i++) {
		FragmentColor += pow(InputImage.SampleLevel(samLinear, index + (float2(0.0f, offset[i]) / float2((fViewportDimensions).x,(fViewportDimensions).y)), 0), 1.0f) * weight[i];
		FragmentColor += pow(InputImage.SampleLevel(samLinear, index - (float2(0.0f, offset[i]) / float2((fViewportDimensions).x,(fViewportDimensions).y)), 0), 1.0f) * weight[i];
	}
  
	blurTex[indexColor] = pow(FragmentColor, 1.0f/1.0f);
}
