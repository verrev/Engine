#pragma once
#include "DirectX11Core.h"
#include "../MC/Filestructure.h"

class Renderable
{
public:
	bool initialize(std::ifstream &file, const std::vector<ID3D11SamplerState*> &samplerStates, const std::vector<ID3D11ShaderResourceView*> &textureSRVs, std::vector<PhongMaterial> &phongMaterials, std::vector<LambertMaterial> &lambertMaterials);
	void draw();
private:
	ID3D11Buffer *mVBuffer;
	ID3D11Buffer *mIBuffer;
	ID3D11Buffer *mCBuffer;
	ID3D11InputLayout *mInputLayout;
	ID3D11PixelShader *mPShader;
	ID3D11VertexShader *mVShader;
	ID3D11SamplerState *mSamplerState;
	ID3D11ShaderResourceView *mTextureSRV;
	UINT mVerticeCount, mIndiceCount, mStride, mOffset;
	CBufferMaterial mCBData;
};