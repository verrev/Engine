#pragma once
#include <Windows.h>
#include <iostream>
#include <DirectXMath.h>
using namespace DirectX;

struct CBufferWorld
{
	XMMATRIX world;
};
struct CBufferMaterial
{
	UINT materialType;
	XMFLOAT3 ambient;

	XMFLOAT3 diffuse;
	float pad0;

	XMFLOAT3 specular;
	float pad1;

	XMFLOAT3 emissive;
	float pad2;

	float alpha;
	float shininess;
	float reflectivity;
	float pad3;

};

enum VERTEX_ELEMENT
{
	POSITION = 1,
	NORMAL = 2,
	TEXCOORD = 4
};
#define MAX_ELEMENT_COUNT 3

struct ModelHeader
{
	UINT meshCount;
};
struct MeshHeader
{
	UINT vsInputLayout;
	char vshaderPath[75];
	char pshaderPath[75];
	UINT verticeCount;
	UINT indiceCount;
	UINT materialID;
};
struct MaterialsHeader
{
	UINT textureCount;
	UINT phongMaterialCount;
	UINT lambertMaterialCount;
};
struct PhongMaterial
{
	PhongMaterial(XMFLOAT3 amb, XMFLOAT3 dif, XMFLOAT3 spec, XMFLOAT3 emi, float alph, float shin, float reflect) :
		ambient(amb), diffuse(dif), specular(spec), emissive(emi), alpha(alph), shininess(shin), reflectivity(reflect) {}
	PhongMaterial() {}
	//--
	friend std::ostream& operator<< (std::ostream& stream, const PhongMaterial& m) {
		stream << "\nPhongMaterial: "
			<< m.ambient.x << " " << m.ambient.y << " " << m.ambient.z << "\n"
			<< m.diffuse.x << " " << m.diffuse.y << " " << m.diffuse.z << "\n"
			<< m.specular.x << " " << m.specular.y << " " << m.specular.z << "\n"
			<< m.emissive.x << " " << m.emissive.y << " " << m.emissive.z << "\n"
			<< m.alpha << " " << m.shininess << " " << m.reflectivity << "\n";
		return stream;
	}
	//--
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 specular;
	XMFLOAT3 emissive;
	float alpha;
	float shininess;
	float reflectivity;
};

struct LambertMaterial
{
	LambertMaterial(XMFLOAT3 amb, XMFLOAT3 dif, XMFLOAT3 emi, float alph) :
		ambient(amb), diffuse(dif), emissive(emi), alpha(alph) {}
	LambertMaterial() {}
	//--
	friend std::ostream& operator<< (std::ostream& stream, const LambertMaterial& m) {
		stream << "\nLambertMaterial: "
			<< m.ambient.x << " " << m.ambient.y << " " << m.ambient.z << "\n"
			<< m.diffuse.x << " " << m.diffuse.y << " " << m.diffuse.z << "\n"
			<< m.emissive.x << " " << m.emissive.y << " " << m.emissive.z << "\n"
			<< m.alpha << "\n";
		return stream;
	}
	//--
	XMFLOAT3 ambient;
	XMFLOAT3 diffuse;
	XMFLOAT3 emissive;
	float alpha;
};


struct Vertex1P
{
	Vertex1P(float x, float y, float z) : pos(XMFLOAT3(x, y, z)) {}
	Vertex1P(XMFLOAT3 p) : pos(p) {}
	Vertex1P() : pos(XMFLOAT3(-1, -1, -1)) {}

	XMFLOAT3 pos;
};
struct Vertex1P1UV
{
	Vertex1P1UV(float x, float y, float z, float u, float v)
		: pos(XMFLOAT3(x, y, z)), texcoord(XMFLOAT2(u, v)) {}
	Vertex1P1UV(XMFLOAT3 p, XMFLOAT2 texc)
		: pos(p), texcoord(texc) {}
	Vertex1P1UV()
		: pos(XMFLOAT3(-1, -1, -1)), texcoord(XMFLOAT2(-1, -1)) {}

	XMFLOAT3 pos;
	XMFLOAT2 texcoord;
};
struct Vertex1P1N1UV
{
	Vertex1P1N1UV(float x, float y, float z, float nx, float ny, float nz, float u, float v)
		: pos(XMFLOAT3(x, y, z)), normal(XMFLOAT3(nx, ny, nz)), texcoord(XMFLOAT2(u, v)) {}
	Vertex1P1N1UV(XMFLOAT3 p, XMFLOAT3 nor, XMFLOAT2 texc)
		: pos(p), normal(nor), texcoord(texc) {}
	Vertex1P1N1UV()
		: pos(XMFLOAT3(-1, -1, -1)), normal(XMFLOAT3(-1, -1, -1)), texcoord(XMFLOAT2(-1, -1)) {}

	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 texcoord;
};