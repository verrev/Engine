#include "Sky.h"
bool Sky::initialize()
{
	Vertex1P1UV v[] =
	{
		Vertex1P1UV(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex1P1UV(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex1P1UV(1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex1P1UV(1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		Vertex1P1UV(-1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
		Vertex1P1UV(1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
		Vertex1P1UV(1.0f,  1.0f, 1.0f, 0.0f, 0.0f),
		Vertex1P1UV(-1.0f,  1.0f, 1.0f, 1.0f, 0.0f),
		Vertex1P1UV(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f),
		Vertex1P1UV(-1.0f, 1.0f,  1.0f, 0.0f, 0.0f),
		Vertex1P1UV(1.0f, 1.0f,  1.0f, 1.0f, 0.0f),
		Vertex1P1UV(1.0f, 1.0f, -1.0f, 1.0f, 1.0f),
		Vertex1P1UV(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		Vertex1P1UV(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex1P1UV(1.0f, -1.0f,  1.0f, 0.0f, 0.0f),
		Vertex1P1UV(-1.0f, -1.0f,  1.0f, 1.0f, 0.0f),
		Vertex1P1UV(-1.0f, -1.0f,  1.0f, 0.0f, 1.0f),
		Vertex1P1UV(-1.0f,  1.0f,  1.0f, 0.0f, 0.0f),
		Vertex1P1UV(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		Vertex1P1UV(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
		Vertex1P1UV(1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		Vertex1P1UV(1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		Vertex1P1UV(1.0f,  1.0f,  1.0f, 1.0f, 0.0f),
		Vertex1P1UV(1.0f, -1.0f,  1.0f, 1.0f, 1.0f),
	};

	UINT i[] =
	{
		0,  1,  2,
		0,  2,  3,
		4,  5,  6,
		4,  6,  7,
		8,  9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23
	};
	mStride = sizeof(Vertex1P1UV);
	mSamplerState = nullptr;
	mTextureSRV = nullptr;
	mVerticeCount = ARRAYSIZE(v);
	mIndiceCount = ARRAYSIZE(i);

	D3D11_INPUT_ELEMENT_DESC ied[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 0, D3D11_INPUT_PER_VERTEX_DATA , 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT , 0, 12, D3D11_INPUT_PER_VERTEX_DATA , 0 },
	};

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	mSamplerState = DirectX11Core::createSamplerState(&sampDesc);
	mTextureSRV = DirectX11Core::createCubeTextureSRVFromFile("res/skymap.dds");

	mVBuffer = DirectX11Core::createVertexBuffer(mStride * mVerticeCount, 0, 0, v);

	mIBuffer = DirectX11Core::createIndexBuffer(sizeof(UINT) * mIndiceCount, 0, i);

	ID3DBlob *vsByteCode = DirectX11Core::readShaderFromFile("res/SKY_VS.cso");
	mInputLayout = DirectX11Core::createInputLayout(vsByteCode, ied, ARRAYSIZE(ied));
	mVShader = DirectX11Core::createVertexShader(vsByteCode);

	ID3DBlob *psByteCode = DirectX11Core::readShaderFromFile("res/SKY_PS.cso");
	mPShader = DirectX11Core::createPixelShader(psByteCode);
	mCBData.world = XMMatrixIdentity();
	mCBData.world = XMMatrixTranspose(mCBData.world);
	mCBuffer = DirectX11Core::createConstantBuffer(sizeof(CBufferWorld), 1, 0, &mCBData);


	D3D11_RASTERIZER_DESC rDesc;
	ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.CullMode = D3D11_CULL_NONE;
	HRESULT hr = DirectX11Core::mDevice->CreateRasterizerState(&rDesc, &RSCullNone);
	if (FAILED(hr)) {
		LOG("CreateRasterizerState");
		return 0;
	}
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = DirectX11Core::mDevice->CreateDepthStencilState(&dssDesc, &DSLessEqual);
	if (FAILED(hr)) {
		LOG("CreateDepthStencilState");
		return 0;
	}
	return true;
}
void Sky::draw(const XMFLOAT3 &camPos)
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
	mCBData.world = XMMatrixTranspose(XMMatrixTranslation(camPos.x, camPos.y + 0.1f, camPos.z));
	DirectX11Core::mDeviceContext->UpdateSubresource(mCBuffer, 0, 0, &mCBData, 0, 0);
	
	ID3D11Buffer *cbuffers[] = { mCBuffer };
	DirectX11Core::mDeviceContext->VSSetConstantBuffers(1, 1, cbuffers);

	if (mTextureSRV) {
		DirectX11Core::mDeviceContext->PSSetShaderResources(0, 1, &mTextureSRV);
	}
	if (mSamplerState) {
		DirectX11Core::mDeviceContext->PSSetSamplers(0, 1, &mSamplerState);
	}

	DirectX11Core::mDeviceContext->OMSetDepthStencilState(DSLessEqual, 0);
	DirectX11Core::mDeviceContext->RSSetState(RSCullNone);


	DirectX11Core::mDeviceContext->DrawIndexed(mIndiceCount, 0, 0);

	DirectX11Core::mDeviceContext->RSSetState(nullptr);
	DirectX11Core::mDeviceContext->OMSetDepthStencilState(nullptr, 0);
}