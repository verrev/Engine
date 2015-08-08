#include "Model.h"
#include "Fileutils.h"

bool Model::initialize(const std::string &modelPath)
{
	mFilePath = modelPath;

	std::ifstream file;
	if (!Fileutils::readFile(mFilePath, file)) {
		LOG("error loading model from file");
		return 0;
	}

	MaterialsHeader matHeader;
	file.read((char*)&matHeader, sizeof(MaterialsHeader));

	for (int k = 0; k < matHeader.textureCount; ++k) {
		char path[75];
		file.read(path, sizeof(char) * 75);
		mSamplerStates.push_back(DirectX11Core::createSamplerState());
		mTextureSRVs.push_back(DirectX11Core::createShaderResourceViewFromFile(path));
	}
	for (int k = 0; k < matHeader.phongMaterialCount; ++k) {
		PhongMaterial phm;
		file.read((char*)&phm, sizeof(PhongMaterial));
		mPhongMaterials.push_back(phm);
	}
	for (int k = 0; k < matHeader.lambertMaterialCount; ++k) {
		LambertMaterial lm;
		file.read((char*)&lm, sizeof(LambertMaterial));
		mLambertMaterials.push_back(lm);
	}
	
	ModelHeader modelHeader;
	file.read((char*)&modelHeader, sizeof(ModelHeader));

	for (UINT i = 0; i < modelHeader.meshCount; ++i) {
		Renderable r;
		r.initialize(file, mSamplerStates, mTextureSRVs, mPhongMaterials, mLambertMaterials);
		mMeshes.push_back(r);
	}
	
	file.read((char*)&mCBData.world, sizeof(XMMATRIX));
	file.close();

	mCBData.world = XMMatrixTranspose(mCBData.world);
	mCBuffer = DirectX11Core::createConstantBuffer(sizeof(CBufferWorld), 1, 0, &mCBData);
}
void Model::draw()
{
	DirectX11Core::mDeviceContext->UpdateSubresource(mCBuffer, 0, 0, &mCBData, 0, 0);

	ID3D11Buffer *cbuffers[] = { mCBuffer };
	DirectX11Core::mDeviceContext->VSSetConstantBuffers(1, 1, cbuffers);
	for (int i = 0; i < mMeshes.size(); ++i) {
		mMeshes[i].draw();
	}
	for (auto m : mMeshes) {
		//m.draw();
	}
}
