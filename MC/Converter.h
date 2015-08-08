#pragma once
#include "Filestructure.h"
#include "Fileutils.h"
#include <string>
#include <vector>
#include <fbxsdk.h>
#pragma comment(lib,"libfbxsdk-md.lib")
#include <iostream>

#ifdef _DEBUG
#define LOG(x) MessageBox(0, TEXT(x), L"Error", MB_OK);
#else
#define LOG(x)
#endif

class Converter
{
public:
	static void convertScene(std::string scenePath);
private:
	static UINT getMaterialIndex(FbxMesh *m);
	static void getIndices(std::vector<UINT> &indices, UINT vcount);
	static void getMeshes(FbxNode *node, std::vector<FbxMesh*> &meshes);
	static void getVertices(FbxMesh* m, std::vector<Vertex1P1N1UV> &vertices);
	static void getMaterials(FbxNode *n, std::vector<std::string> &textures, std::vector<PhongMaterial> &phongMats, std::vector<LambertMaterial> &lambertMats);
	static void convertModel(FbxNode *rootNode, std::string outPath);
	static void getNormal(FbxMesh* inMesh, int inCtrlPointIndex, int inVertexCounter, XMFLOAT3& outNormal);
	static void getUV(FbxMesh* m, int i, int j, XMFLOAT2& outUV);
	static void getDiffuseTextures(FbxSurfaceMaterial *material, std::vector<std::string> &textures);
	static PhongMaterial getPhongMaterial(FbxSurfaceMaterial *material);
	static LambertMaterial getLambertMaterial(FbxSurfaceMaterial *material);
};