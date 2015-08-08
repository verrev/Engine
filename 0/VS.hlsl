cbuffer CBWorld : register(b0)
{
	matrix world;
};
cbuffer CBProj : register(b1)
{
	matrix proj;
};
cbuffer CBView : register(b2)
{
	matrix view;
};
float4 main(float3 pos : POSITION) : SV_POSITION
{
	float4 outPos = mul(float4(pos, 1.0f), world);
	outPos = mul(outPos, view);
	outPos = mul(outPos, proj);
	return outPos;
}


