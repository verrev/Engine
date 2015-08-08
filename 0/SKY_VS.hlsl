cbuffer CBCamera : register(b0)
{
	matrix viewProj;
};
cbuffer CBWorld : register(b1)
{
	matrix world;
};
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 tex : TEXCOORD;
};

VS_OUTPUT main(float3 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(float4(inPos, 1.0f), world);
	output.pos = mul(output.pos, viewProj).xyww;
	output.tex = inPos;
	return output;
}