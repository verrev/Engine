#include "Renderable.h"
#include "Fileutils.h"
#include "ResourceParser.h"

bool Renderable::initialize(std::ifstream &file, const std::vector<ID3D11SamplerState*> &samplerStates, const std::vector<ID3D11ShaderResourceView*> &textureSRVs, std::vector<PhongMaterial> &phongMaterials, std::vector<LambertMaterial> &lambertMaterials)
{
	mSamplerState = nullptr;
	mTextureSRV = nullptr;
	mOffset = 0;

	MeshHeader meshHeader;
	file.read((char*)&meshHeader, sizeof(MeshHeader));

	mVerticeCount = meshHeader.verticeCount;
	mIndiceCount = meshHeader.indiceCount;

	D3D11_INPUT_ELEMENT_DESC ied[MAX_ELEMENT_COUNT];
	UINT elementCount = 0;
	ResourceParser::getIEDfromHeader(meshHeader, ied, mStride, elementCount);

	std::vector<Vertex1P> v1P;
	std::vector<Vertex1P1N1UV> v1P1N1UV;
	void *v = nullptr;

	if (meshHeader.vsInputLayout & VERTEX_ELEMENT::POSITION &&
		meshHeader.vsInputLayout & VERTEX_ELEMENT::TEXCOORD &&
		meshHeader.vsInputLayout & VERTEX_ELEMENT::NORMAL) {
		v1P1N1UV.resize(mVerticeCount);
		v = &v1P1N1UV[0];
	}
	else {
		v1P.resize(mVerticeCount);
		v = &v1P[0];
	}

	file.read((char*)v, mVerticeCount * mStride);
	mVBuffer = DirectX11Core::createVertexBuffer(mStride * mVerticeCount, 0, 0, v);

	std::vector<UINT> i;
	i.resize(mIndiceCount);
	file.read((char*)&i[0], mIndiceCount * sizeof(UINT));
	mIBuffer = DirectX11Core::createIndexBuffer(sizeof(UINT) * mIndiceCount, 0, &i[0]);

	ID3DBlob *vsByteCode = DirectX11Core::readShaderFromFile(meshHeader.vshaderPath);
	mInputLayout = DirectX11Core::createInputLayout(vsByteCode, ied, elementCount);
	mVShader = DirectX11Core::createVertexShader(vsByteCode);

	ID3DBlob *psByteCode = DirectX11Core::readShaderFromFile(meshHeader.pshaderPath);
	mPShader = DirectX11Core::createPixelShader(psByteCode);


	if (phongMaterials.size() > meshHeader.materialID) {
		auto mat = phongMaterials[meshHeader.materialID];
		mCBData.alpha = mat.alpha;
		mCBData.ambient = mat.ambient;
		mCBData.diffuse = mat.diffuse;
		mCBData.emissive = mat.emissive;
		mCBData.materialType = 0; // ENUM THIS
		mCBData.reflectivity = mat.reflectivity;
		mCBData.shininess = mat.shininess;
		mCBData.specular = mat.specular;
	}
	if (meshHeader.vsInputLayout & VERTEX_ELEMENT::TEXCOORD) {

		// LAMBERT? SINGLE LIST FOR ALL MATS, GUID for ID
		if (samplerStates.size() > 0)
			mSamplerState = samplerStates[0];
		else
			mSamplerState = nullptr;
		if (textureSRVs.size() > meshHeader.materialID) {
			mTextureSRV = textureSRVs[meshHeader.materialID];
			mCBData.materialType = 1;
		}
		else
			mTextureSRV = nullptr;
	}
	mCBuffer = DirectX11Core::createConstantBuffer(sizeof(CBufferMaterial), 0, 0, &mCBData);

	return true;
}
void Renderable::draw()
{
	ID3D11Buffer *vbuffers[] = { mVBuffer };
	UINT strides[] = { mStride };
	UINT offsets[] = { mOffset };
	DirectX11Core::mDeviceContext->IASetVertexBuffers(0, 1, vbuffers, strides, offsets);

	DirectX11Core::mDeviceContext->IASetIndexBuffer(mIBuffer, DXGI_FORMAT_R32_UINT, 0);

	DirectX11Core::mDeviceContext->IASetInputLayout(mInputLayout);
	DirectX11Core::mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DirectX11Core::mDeviceContext->VSSetShader(mVShader, nullptr, 0);
	DirectX11Core::mDeviceContext->PSSetShader(mPShader, nullptr, 0);

	DirectX11Core::mDeviceContext->PSSetConstantBuffers(0, 1, &mCBuffer);

	if (1) { // mTextureSRV
		DirectX11Core::mDeviceContext->PSSetShaderResources(0, 1, &mTextureSRV);
	}
	if (1) { //mSamplerState
		DirectX11Core::mDeviceContext->PSSetSamplers(0, 1, &mSamplerState);
	}

	DirectX11Core::mDeviceContext->DrawIndexed(mIndiceCount, 0, 0);
}