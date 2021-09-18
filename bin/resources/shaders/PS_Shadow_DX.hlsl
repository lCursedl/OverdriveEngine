struct PS_INPUT {
	float4 psPos	: SV_POSITION;
	float depthPos	: TEXCOORD0;
};

float4 main(PS_INPUT Input) : SV_Target {
	return float4(Input.depthPos, Input.depthPos, Input.depthPos, 1.0f);
}