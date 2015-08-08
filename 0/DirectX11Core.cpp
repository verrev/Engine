#include "DirectX11Core.h"
// FIND A BETTER WAY?
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx11.h"
#pragma comment(lib,"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3dx11.lib")

ID3D11Device *DirectX11Core::mDevice = nullptr;
ID3D11DeviceContext *DirectX11Core::mDeviceContext = nullptr;
IDXGISwapChain *DirectX11Core::mSwapChain = nullptr;
ID3D11Texture2D *DirectX11Core::mSwapChainBuffer = nullptr;
ID3D11Debug *DirectX11Core::mDebugger = nullptr;
ID3D11RenderTargetView *DirectX11Core::mRenderTargetView = nullptr;
ID3D11Texture2D *DirectX11Core::mDepthStencilBuffer = nullptr;
ID3D11DepthStencilView *DirectX11Core::mDepthStencilView = nullptr;

UINT DirectX11Core::mWindowWidth = 100;
UINT DirectX11Core::mWindowHeight = 100;
HWND DirectX11Core::mWindow = 0; 
bool DirectX11Core::mWindowed = 1;

std::vector<ID3D11Buffer*> DirectX11Core::mBuffers;
std::vector<ID3D11VertexShader*> DirectX11Core::mVShaders;
std::vector<ID3D11PixelShader*> DirectX11Core::mPShaders;
std::vector<ID3D11InputLayout*> DirectX11Core::mInputLayouts;
std::vector<ID3D11SamplerState*> DirectX11Core::mSamplerStates;
std::vector<ID3D11ShaderResourceView*> DirectX11Core::mShaderResourceViews;

bool DirectX11Core::initialize(const UINT &width, const UINT &height, const HWND &window, const bool &windowed)
{
	mWindowWidth = width;
	mWindowHeight = height;
	mWindow = window;
	mWindowed = windowed;


	/* MSAA check
	UINT quality;
	hr = device->CheckMultisampleQualityLevels(DXGI_FORMAT_D32_FLOAT, 1, &quality);
	if (FAILED(hr)) {
	LOG("DXGI_FORMAT_D32_FLOAT MSAA 1");
	return;
	}
	quality -= 1;
	*/

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0 };

	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferDesc.Width = mWindowWidth;
	scd.BufferDesc.Height = mWindowHeight;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	scd.BufferCount = 1;
	scd.OutputWindow = mWindow;
	scd.Windowed = windowed;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HRESULT hr = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, flags, featureLevels, 2, D3D11_SDK_VERSION, &scd, &mSwapChain, &mDevice, 0, &mDeviceContext);
	if (FAILED(hr)) {
		LOG("D3D11CreateDeviceAndSwapChain");
		return 0;
	}

	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&mSwapChainBuffer);
	if (FAILED(hr)) {
		LOG("GetBuffer");
		return 0;
	}

	hr = mDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&mDebugger);
	if (FAILED(hr)) {
		LOG("QueryInterface");
		return 0;
	}

	hr = mDevice->CreateRenderTargetView(mSwapChainBuffer, 0, &mRenderTargetView);
	if (FAILED(hr)) {
		LOG("CreateRenderTargetView");
		return 0;
	}

	D3D11_TEXTURE2D_DESC dsdDesc;
	dsdDesc.Width = mWindowWidth;
	dsdDesc.Height = mWindowHeight;
	dsdDesc.MipLevels = 1;
	dsdDesc.ArraySize = 1;
	dsdDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsdDesc.SampleDesc.Count = 1;
	dsdDesc.SampleDesc.Quality = 0;
	dsdDesc.Usage = D3D11_USAGE_DEFAULT;
	dsdDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsdDesc.CPUAccessFlags = 0;
	dsdDesc.MiscFlags = 0;

	hr = mDevice->CreateTexture2D(&dsdDesc, 0, &mDepthStencilBuffer);
	if (FAILED(hr)) {
		LOG("CreateTexture2D");
		return 0;
	}

	hr = mDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);
	if (FAILED(hr)) {
		LOG("CreateDepthStencilView");
		return 0;
	}

	D3D11_VIEWPORT vp = { 0 };
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<FLOAT>(mWindowWidth);
	vp.Height = static_cast<FLOAT>(mWindowHeight);
	mDeviceContext->RSSetViewports(1, &vp);

	setRenderTargetViews();

	return true;
}
void DirectX11Core::setRenderTargetViews()
{
	ID3D11RenderTargetView *renderTargetViews[] = { DirectX11Core::mRenderTargetView };
	ID3D11DepthStencilView *depthTargetView = DirectX11Core::mDepthStencilView;
	DirectX11Core::mDeviceContext->OMSetRenderTargets(1, renderTargetViews, depthTargetView);
}
void DirectX11Core::endScene()
{
	mSwapChain->Present(0, 0);
}
void DirectX11Core::clearRenderTargetViews(const float &r, const float &g, const float &b, const float &a)
{
	ID3D11RenderTargetView *rtvs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { nullptr };
	ID3D11DepthStencilView *dtv = nullptr;
	DirectX11Core::mDeviceContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, rtvs, &dtv);
	for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
		if (rtvs[i] != nullptr) {
			float clearCols[] = { r, g, b, a };
			DirectX11Core::mDeviceContext->ClearRenderTargetView(rtvs[i], clearCols);
			safeRelease(rtvs[i]);
		}
	}
	if (dtv != nullptr) {
		DirectX11Core::mDeviceContext->ClearDepthStencilView(dtv, D3D11_CLEAR_STENCIL | D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
	safeRelease(dtv);
}
void DirectX11Core::destroy()
{
	mSwapChain->SetFullscreenState(0, 0);

	mDeviceContext->ClearState();
	safeRelease(mDevice);
	safeRelease(mDeviceContext);
	safeRelease(mSwapChain);
	safeRelease(mSwapChainBuffer);
	safeRelease(mDebugger);
	safeRelease(mRenderTargetView);
	safeRelease(mDepthStencilBuffer);
	safeRelease(mDepthStencilView);

	for (auto b : mBuffers) safeRelease(b);
	for (auto vs : mVShaders) safeRelease(vs);
	for (auto ps : mPShaders) safeRelease(ps);
	for (auto il : mInputLayouts) safeRelease(il);
	for (auto ss : mSamplerStates) safeRelease(ss);
	for (auto srv : mShaderResourceViews) safeRelease(srv);
}

ID3D11Buffer *DirectX11Core::createVertexBuffer(const UINT &size, const bool &dynamic, const bool &streamOut, const void *const data)
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = size;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	if (streamOut)
		desc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;

	if (dynamic) {
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else {
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0;
	}

	D3D11_SUBRESOURCE_DATA srd;
	srd.pSysMem = data;

	mBuffers.push_back(nullptr);
	HRESULT hr = mDevice->CreateBuffer(&desc, &srd, &mBuffers[mBuffers.size() - 1]);
	if (FAILED(hr)) {
		LOG("createVertexBuffer");
		return nullptr;
	}
	return mBuffers[mBuffers.size() - 1];
}
ID3D11Buffer *DirectX11Core::createIndexBuffer(const UINT &size, const bool &dynamic, const void *const data)
{
	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = size;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	if (dynamic) {
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else {
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0;
	}
	
	D3D11_SUBRESOURCE_DATA srd;
	srd.pSysMem = data;

	mBuffers.push_back(nullptr);
	HRESULT hr = mDevice->CreateBuffer(&desc, &srd, &mBuffers[mBuffers.size() - 1]);
	if (FAILED(hr)) {
		LOG("createIndexBuffer");
		return nullptr;
	}
	return mBuffers[mBuffers.size() - 1];
}
ID3D11Buffer *DirectX11Core::createConstantBuffer(const UINT &size, const bool &dynamic, const bool &updateByCPU, const void *const data)
{
	if (size % 16) {
		LOG("createConstantBuffer - buffer size is not a multiple of 16");
		return nullptr;
	}

	D3D11_BUFFER_DESC desc = { 0 };
	desc.ByteWidth = size;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (dynamic && updateByCPU) {
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (dynamic && !updateByCPU) {
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
	}
	else {
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.CPUAccessFlags = 0;
	}

	D3D11_SUBRESOURCE_DATA srd;
	srd.pSysMem = data;
	
	mBuffers.push_back(nullptr);
	HRESULT hr = mDevice->CreateBuffer(&desc, &srd, &mBuffers[mBuffers.size() - 1]);
	if (FAILED(hr)) {
		LOG("createConstantBuffer");
		return nullptr;
	}
	return mBuffers[mBuffers.size() - 1];
}
ID3D11VertexShader *DirectX11Core::createVertexShader(ID3DBlob *vsByteCode)
{
	mVShaders.push_back(nullptr);
	
	HRESULT hr = mDevice->CreateVertexShader(vsByteCode->GetBufferPointer(), vsByteCode->GetBufferSize(), nullptr, &mVShaders[mVShaders.size() - 1]);
	if (FAILED(hr)) {
		LOG("createVertexShader failed");
		return nullptr;
	}
	return mVShaders[mVShaders.size() - 1];
}
ID3D11PixelShader *DirectX11Core::createPixelShader(ID3DBlob *psByteCode)
{
	mPShaders.push_back(nullptr);

	HRESULT hr = mDevice->CreatePixelShader(psByteCode->GetBufferPointer(), psByteCode->GetBufferSize(), nullptr, &mPShaders[mPShaders.size() - 1]);
	if (FAILED(hr)) {
		LOG("createPixelShader failed");
		return nullptr;
	}
	return mPShaders[mPShaders.size() - 1];
}
ID3D11InputLayout *DirectX11Core::createInputLayout(ID3DBlob *vsByteCode, const D3D11_INPUT_ELEMENT_DESC *const ied, const UINT &elementCount)
{
	mInputLayouts.push_back(nullptr);

	HRESULT hr = mDevice->CreateInputLayout(ied, elementCount, vsByteCode->GetBufferPointer(), vsByteCode->GetBufferSize(), &mInputLayouts[mInputLayouts.size() - 1]);
	if (FAILED(hr)) {
		LOG("CreateInputLayout");
		return nullptr;
	}
	return mInputLayouts[mInputLayouts.size() - 1];
}
ID3DBlob *DirectX11Core::readShaderFromFile(const std::string &shaderPath)
{
	std::wstring wsPath;
	wsPath.assign(shaderPath.begin(), shaderPath.end());
	
	ID3DBlob *byteCode = nullptr;
	HRESULT hr = D3DReadFileToBlob(wsPath.c_str(), &byteCode);
	if (FAILED(hr)) {
		LOG("readShaderFromFile failed");
		return nullptr;
	}
	return byteCode;
}
ID3D11SamplerState *DirectX11Core::createSamplerState(const D3D11_SAMPLER_DESC *const sd)
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	if (sd != nullptr) {
		samplerDesc = *sd;
	}
	else {
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	}
	mSamplerStates.push_back(nullptr);
	HRESULT hr = mDevice->CreateSamplerState(&samplerDesc, &mSamplerStates[mSamplerStates.size() - 1]);
	if (FAILED(hr)) {
		LOG("createSamplerState failed");
		return nullptr;
	}

	return mSamplerStates[mSamplerStates.size() - 1];
}
ID3D11ShaderResourceView *DirectX11Core::createShaderResourceViewFromFile(const std::string &filePath)
{
	std::wstring ws;
	ws.assign(filePath.begin(), filePath.end());

	HRESULT hr;
	mShaderResourceViews.push_back(nullptr);
	D3DX11CreateShaderResourceViewFromFile(DirectX11Core::mDevice, ws.c_str(), nullptr, 0, &mShaderResourceViews[mShaderResourceViews.size() - 1], &hr);
	if (FAILED(hr)) {
		LOG("createShaderResourceViewFromFile failed");
		return nullptr;
	}

	return mShaderResourceViews[mShaderResourceViews.size() - 1];
}
ID3D11ShaderResourceView *DirectX11Core::createCubeTextureSRVFromFile(const std::string &filePath)
{
	std::wstring ws;
	ws.assign(filePath.begin(), filePath.end());

	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	ID3D11Texture2D* SMTexture = 0;
	HRESULT hr = D3DX11CreateTextureFromFile(mDevice, ws.c_str(), &loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);
	if (FAILED(hr)) {
		LOG("D3DX11CreateTextureFromFile failed");
		return nullptr;
	}

	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;
	
	mShaderResourceViews.push_back(nullptr);
	hr = mDevice->CreateShaderResourceView(SMTexture, &SMViewDesc, &mShaderResourceViews[mShaderResourceViews.size() - 1]);
	if (FAILED(hr)) {
		safeRelease(SMTexture);
		LOG("createShaderResourceViewFromFile failed");
		return nullptr;
	}
	safeRelease(SMTexture);
	return mShaderResourceViews[mShaderResourceViews.size() - 1];
}