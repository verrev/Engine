struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
};
float4 main(VS_OUTPUT input) : SV_TARGET
{
	return float4(1,1,1,1);
	//return float4(ColorTexture.Sample(LinearSampler, input.tex));
}