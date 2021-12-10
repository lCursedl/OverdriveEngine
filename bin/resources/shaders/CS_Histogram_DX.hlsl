//Render target
RWTexture2D<float4> histogramTex : register(u0);
//Texture objects
Texture2D rendererTex : register(t0);
//Buffer
RWBuffer<int> dataR : register(u1);
RWBuffer<int> dataG : register(u2);
RWBuffer<int> dataB : register(u3);
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
[numthreads(32, 32, 1)]
	void
	main(uint3 groupThreadID : SV_GroupThreadID,
		  uint3 groupID : SV_GroupID,
		  uint3 dispatchThreadID : SV_DispatchThreadID,
		  uint groupIndex : SV_GroupIndex) {
	
	if (dispatchThreadID.x > 800 || dispatchThreadID.y > 600) {
		return;
	}
	
	uint2 indexColor = dispatchThreadID.xy;
	float3 texel = rendererTex.Load(int3(indexColor, 0)).xyz;
	int3 texelColor = int3(mul(texel, 256.0f));
	InterlockedAdd(dataR[texelColor.x], 1);
	InterlockedAdd(dataG[texelColor.y], 1);
	InterlockedAdd(dataB[texelColor.z], 1);
	
	//Syncrhonize Barriers HLSL
	
	DeviceMemoryBarrier();
	float2 uv = float2(indexColor) / float2(800, 600);
	int pIndex = int(floor(uv.x * 256.0f));
	
	float rFreq = dataR[pIndex];
	float gFreq = dataG[pIndex];
	float bFreq = dataB[pIndex];
	
	float val = uv.y * 5000.0f;
	
	float rResult = step(val, rFreq);
	float gResult = step(val, gFreq);
	float bResult = step(val, bFreq);
	
	histogramTex[indexColor] = float4(rResult, gResult, bResult, 1.0f);
}