#pragma once
#include "Renderable.h"

class Model
{
public:
	bool initialize(const std::string &modelPath);
	void draw();
	inline std::string getFilePath() const
	{
		return mFilePath;
	}
	inline void setWorldMatrix(const XMMATRIX &w, const bool &transpose = 1)
	{
		if (transpose)
			mCBData.world = XMMatrixTranspose(w);
		else
			mCBData.world = w;
	}
	inline XMMATRIX getWorldMatrix()
	{
		return XMMatrixTranspose(mCBData.world);
	}
private:
	std::string mFilePath;
	std::vector<Renderable> mMeshes;
	ID3D11Buffer *mCBuffer;
	CBufferWorld mCBData;
	std::vector<ID3D11SamplerState*> mSamplerStates;
	std::vector<ID3D11ShaderResourceView*> mTextureSRVs;
	std::vector<PhongMaterial> mPhongMaterials;
	std::vector<LambertMaterial> mLambertMaterials;
};