Texture2D ColorTexture : register(t0);
SamplerState LinearSampler : register(s0);

cbuffer CBMaterial : register(b0)
{
	int materialType;
	float3 ambient;

	float3 diffuse;
	float pad0;

	float3 specular;
	float pad1;

	float3 emissive;
	float pad2;

	float alpha;
	float shininess;
	float reflectivity;
	float pad3;
};

cbuffer CBLight : register (b1)
{
	float3 gAmbient;
	float pad4;

	float3 gDiffuse;
	float pad5;

	float3 gSpecular;
	float pad6;

	float3 gLightDir;
	float pad7;

	float3 gEyePos;
	float pad8;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 nor : NORMAL;
	float2 tex : TEXCOORD;
};
//--------------------------------------------------------------------------------
struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};
float4 DoDiffuse(float3 L, float3 N)
{
	float NdotL = max(0, dot(N, L));
	return float4(gDiffuse, 1.0) * NdotL;
}
float4 DoSpecular(float3 V, float3 L, float3 N)
{
	float3 R = normalize(reflect(-L, N));
	float RdotV = max(0, dot(R, V));
	return float4(gSpecular, 1.0) * pow(RdotV, shininess);
}
LightingResult DoDirectionalLight(float3 V, float4 P, float3 N)
{
	LightingResult result;
	float3 L = -gLightDir;
	result.Diffuse = DoDiffuse(L, N);
	result.Specular = DoSpecular(V, L, N);
	return result;
}

LightingResult ComputeLighting(float4 P, float3 N)
{
	float3 V = normalize(float4(gEyePos,1.0) - P).xyz;

	LightingResult totalResult = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };
	LightingResult result = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };
	result = DoDirectionalLight(V, P, N);
	totalResult.Diffuse += result.Diffuse;
	totalResult.Specular += result.Specular;
	totalResult.Diffuse = saturate(totalResult.Diffuse);
	totalResult.Specular = saturate(totalResult.Specular);

	return totalResult;
}
//--------------------------------------------------------------------------------

float4 main(VS_OUTPUT IN) : SV_TARGET
{
	LightingResult lit = ComputeLighting(IN.pos, normalize(IN.nor));

	float4 e = float4(emissive, 1.0);
	float4 a = float4(ambient, 1.0) * float4(gAmbient, 1.0);
	float4 d = float4(diffuse,1.0) * lit.Diffuse;
	float4 s = float4(specular, 1.0) * lit.Specular;


	float4 finalColor = (e + a + d + s);
	if (materialType == 0) {
		finalColor = finalColor;
	}
	else {
		float4 texColor = ColorTexture.Sample(LinearSampler, IN.tex);
		finalColor *= texColor;
	}
	return finalColor;
}
/*
float4 main(VS_OUTPUT input) : SV_TARGET
{
	//return float4(1,1,1,1);

	input.nor = normalize(input.nor);

	float3 finalAmbient = gAmbient * ambient;

	float3 lightVec = float3(0, 0, 1);

	float3 l = normalize(lightVec - input.pos.xyz);
	float3 finalDiffuse = max(0, dot(l, input.nor)) * gDiffuse * diffuse;

	float3 v = normalize(float3(0, -5, 0) - input.pos.xyz);
	float3 r = 2 * (dot(l, input.nor)) * input.nor - l;
	float3 finalSpecular = pow(dot(r, v), shininess) * gSpecular * specular;

	float4 final = float4((emissive + finalAmbient + finalDiffuse + finalSpecular),1.0) * float4(ColorTexture.Sample(LinearSampler, input.tex));

	return final;
	













	// ----------------------------------------------------
	/*
	if (materialType == 0) {
		float3 n = normalize(input.nor);
		float3 l = normalize(input.light);

		float illum = max(dot(l, n), 0);
		float3 difCol = illum * diffuse * float3(0.2,0.2,0.2);
		float3 ambCol = ambient * float3(0.2, 0.2, 0.2);
		float4 surfCol = float4(ColorTexture.Sample(LinearSampler, input.tex));
		
		float3 r = reflect(-l, n);
		float t = pow(max(dot(r, l), 0), shininess);
		float3 spc = t * specular * float3(0.2, 0.2, 0.2);
		 // + spc causes weird things
		return surfCol * float4( illum + difCol + ambCol + spc , 1);
	}
	else {
		return float4(1, 1, 1, 1.0);
	}
}
*/