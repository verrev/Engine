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
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};

VS_OUTPUT main(float3 pos : POSITION, float3 nor : NORMAL, float2 tex : TEXCOORD)
{
	VS_OUTPUT o;
	o.pos = mul(float4(pos, 1.0f), world);
	o.pos = mul(o.pos, viewProj);

	o.nor = mul(nor, (float3x3) world);
	o.tex = tex;
	
	return o;
}


