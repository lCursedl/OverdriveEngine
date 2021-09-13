// //Texture objects
// Texture2D InputTexture : register (t0);

// //Sampler objects
// SamplerState lSampler : register(s0);

// //Constant buffer
// cbuffer Dimensions : register (b0) {
	// float4 ViewportDim;
// };

// //Input structure
// struct PS_INPUT {
	// float4 psPos	 : SV_POSITION;
	// float2 TexCoord : TEXCOORD0;
// };

// float gamma = 1.0f;

// float4 main(PS_INPUT Input) : SV_Target {
	// float offset[3] = {0.0, 1.3846153846, 3.2307692308};
	// float weight[3] = {0.2270270270, 0.3162162162, 0.0702702703};
	// float4 fragmentColor = pow(InputTexture.Sample(lSampler, Input.TexCoord), gamma) * weight[0];
	
	// for(int i = 1; i < 3; ++i) {
		// fragmentColor += pow(InputTexture.Sample(lSampler, Input.TexCoord + (float2(0.0f, offset[i]) / ViewportDim.xy)), gamma) * weight[i];
		// fragmentColor += pow(InputTexture.Sample(lSampler, Input.TexCoord - (float2(0.0f, offset[i]) / ViewportDim.xy)), gamma) * weight[i];
	// }
	// return pow(fragmentColor, 1.0f/1.0f);
// }

Texture2D InputImage : register( t0 ); //Rougnes

//Sampler
SamplerState samLinear : register( s0 );

cbuffer cbBlur : register( b0 )
{
    float4 fViewportDimensions;
};

struct PS_INPUT
{
  float4 Position : SV_POSITION;
  float2 TexCoord : TEXCOORD0;
};

float gamma = 1.0f;  

float4 main(PS_INPUT input) : SV_TARGET0
{
  float offset[3] = {0.0, 1.3846153846, 3.2307692308};
  float weight[3] = {0.2270270270, 0.316216162, 0.0702702703};
  
  float4 FragmentColor = pow(InputImage.Sample(samLinear, input.TexCoord), 1.0f) * weight[0];
  
  for(int i = 1; i < 3; i++)
  {
    FragmentColor += pow(InputImage.Sample(samLinear, input.TexCoord + (float2(0.0f, offset[i]) / float2((fViewportDimensions).x,(fViewportDimensions).y))), 1.0f) * weight[i];
	FragmentColor += pow(InputImage.Sample(samLinear, input.TexCoord - (float2(0.0f, offset[i]) / float2((fViewportDimensions).x,(fViewportDimensions).y))), 1.0f) * weight[i];
  }
  
  return pow(FragmentColor, 1.0f/1.0f);
}