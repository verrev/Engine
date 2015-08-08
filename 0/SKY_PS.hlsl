TextureCube SkyMap;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 tex : TEXCOORD;
};
float4 main(VS_OUTPUT input) : SV_Target
{
	return SkyMap.Sample(ObjSamplerState, input.tex);
}