#include "Converter.h"

void Converter::convertScene(std::string scenePath)
{
	FbxManager* manager = FbxManager::Create();

	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	FbxImporter* importer = FbxImporter::Create(manager, "");
	importer->Initialize(scenePath.c_str(), -1, manager->GetIOSettings());

	FbxScene* scene = FbxScene::Create(manager, "");
	importer->Import(scene);
	importer->Destroy();

	FbxAxisSystem axisSystem = FbxAxisSystem::eDirectX;
	if (scene->GetGlobalSettings().GetAxisSystem() != axisSystem) axisSystem.ConvertScene(scene);

	scenePath.replace(scenePath.find(".FBX"), 4, ".aaa");

	FbxGeometryConverter c(manager);
	c.SplitMeshesPerMaterial(scene, 1);
	convertModel(scene->GetRootNode(), scenePath);
}

void Converter::convertModel(FbxNode *rootNode, std::string outPath)
{
	std::vector<FbxMesh*> meshes;
	getMeshes(rootNode, meshes);

	std::vector<PhongMaterial> phongMats;
	std::vector<LambertMaterial> lambertMats;
	std::vector<std::string> textures;
	getMaterials(meshes[0]->GetNode(), textures, phongMats, lambertMats);

	for (auto a : textures)
		std::cout << a << "\n";
	for (auto a : phongMats)
		std::cout << a << "\n";
	for (auto a : lambertMats)
		std::cout << a << "\n";

	std::cout << "------------------------------------\n";
	std::cout << "Textures: " << textures.size() << "\n";
	std::cout << "Phong: " << phongMats.size() << "\n";
	std::cout << "Lambert: " << lambertMats.size() << "\n";


	MaterialsHeader mh;
	mh.textureCount = textures.size();
	mh.phongMaterialCount = phongMats.size();
	mh.lambertMaterialCount = lambertMats.size();
	Fileutils::writeFile(outPath, &mh, sizeof(MaterialsHeader));

	for (auto a : textures) {
		std::string s = "res/" + a;
		char tex[75];
		strcpy_s(tex, s.c_str());
		Fileutils::writeFile(outPath, tex, sizeof(char) * 75, std::ios::app);
	}
	for (auto a : phongMats) {
		Fileutils::writeFile(outPath, &a, sizeof(PhongMaterial), std::ios::app);
	}
	for (auto a : lambertMats) {
		Fileutils::writeFile(outPath, &a, sizeof(LambertMaterial), std::ios::app);
	}

	ModelHeader modelHeader;
	modelHeader.meshCount = meshes.size();
	Fileutils::writeFile(outPath, &modelHeader, sizeof(ModelHeader), std::ios::app);

	std::cout << "Meshcount: " << modelHeader.meshCount << "\n";

	for (int i = 0; i < modelHeader.meshCount; ++i) {
		std::vector<Vertex1P1N1UV> v;
		getVertices(meshes[i], v);

		MeshHeader h;
		h.verticeCount = v.size();
		h.indiceCount = h.verticeCount;
		h.vsInputLayout = VERTEX_ELEMENT::POSITION | VERTEX_ELEMENT::NORMAL | VERTEX_ELEMENT::TEXCOORD;
		h.materialID = getMaterialIndex(meshes[i]);
		strcpy_s(h.vshaderPath, "res/VS2.cso");
		strcpy_s(h.pshaderPath, "res/PS2.cso");
		Fileutils::writeFile(outPath, &h, sizeof(MeshHeader), std::ios::app);

		Fileutils::writeFile(outPath, &v[0], sizeof(Vertex1P1N1UV) * h.verticeCount, std::ios::app);

		std::vector<UINT> indices;
		getIndices(indices, h.verticeCount);
		Fileutils::writeFile(outPath, &indices[0], sizeof(UINT) * indices.size(), std::ios::app);

	}
	XMMATRIX world = XMMatrixIdentity();
	Fileutils::writeFile(outPath, &world, sizeof(XMMATRIX), std::ios::app);
}
void Converter::getMeshes(FbxNode *node, std::vector<FbxMesh*> &meshes)
{
	for (int i = 0; i < node->GetNodeAttributeCount(); ++i) {
		if (node->GetNodeAttributeByIndex(i)->GetAttributeType() == FbxNodeAttribute::eMesh) {
			meshes.push_back(static_cast<FbxMesh*>(node->GetNodeAttributeByIndex(i)));
		}
	}
	for (int i = 0; i < node->GetChildCount(); ++i) {
		getMeshes(node->GetChild(i), meshes);
	}
}
