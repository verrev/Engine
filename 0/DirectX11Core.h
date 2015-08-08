#pragma once

#include <Windows.h>
#include <vector>
#include <string>
#include <d3d11.h>
#pragma comment (lib,"d3d11.lib")
#include <DirectXMath.h>
using namespace DirectX;
#include <D3Dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#ifdef _DEBUG
//#define LOG(x) MessageBox(0, TEXT(x), L"Error", MB_OK);
#define LOG(x) OutputDebugString(TEXT(x" --ERROR FOUND--\n"));
#else
#define LOG(x)
#endif

template<typename T>
void safeRelease(T obj)
{
	if (obj) obj->Release();
	obj = 0;
}

class DirectX11Core
{
public:
	static bool initialize(const UINT &width, const UINT &height, const HWND &window, const bool &windowed);
	static void setRenderTargetViews();
	static void clearRenderTargetViews(const float &r = 0.0f, const float &g = 0.0f, const float &b = 0.0f, const float &a = 1.0f);
	static void endScene();
	static void destroy();
public:
	static ID3D11Buffer *createVertexBuffer(const UINT &size, const bool &dynamic, const bool &streamOut, const void *const data);
	static ID3D11Buffer *createIndexBuffer(const UINT &size, const bool &dynamic, const void *const data);
	static ID3D11Buffer *createConstantBuffer(const UINT &size, const bool &dynamic, const bool &updateByCPU, const void *const data);
	static ID3D11VertexShader *createVertexShader(ID3DBlob *vsByteCode);
	static ID3D11PixelShader *createPixelShader(ID3DBlob *psByteCode);
	static ID3D11InputLayout *createInputLayout(ID3DBlob *vsByteCode, const D3D11_INPUT_ELEMENT_DESC *const ied, const UINT &elementCount);
	static ID3DBlob *readShaderFromFile(const std::string &shaderPath);
	static ID3D11SamplerState *createSamplerState(const D3D11_SAMPLER_DESC *const sd = nullptr);
	static ID3D11ShaderResourceView *createShaderResourceViewFromFile(const std::string &filePath);
	static ID3D11ShaderResourceView *createCubeTextureSRVFromFile(const std::string &filePath);
public:
	static ID3D11Device *mDevice;
	static ID3D11DeviceContext *mDeviceContext;
	static IDXGISwapChain *mSwapChain;
	static ID3D11Texture2D *mSwapChainBuffer;
	static ID3D11Debug *mDebugger;
	static ID3D11RenderTargetView *mRenderTargetView;
	static ID3D11Texture2D *mDepthStencilBuffer;
	static ID3D11DepthStencilView *mDepthStencilView;
public:
	static UINT mWindowWidth;
	static UINT mWindowHeight;
	static HWND mWindow;
	static bool mWindowed;
private:
	static std::vector<ID3D11Buffer*> mBuffers;
	static std::vector<ID3D11VertexShader*> mVShaders;
	static std::vector<ID3D11PixelShader*> mPShaders;
	static std::vector<ID3D11InputLayout*> mInputLayouts;
	static std::vector<ID3D11SamplerState*> mSamplerStates;
	static std::vector<ID3D11ShaderResourceView*> mShaderResourceViews;
private:
	DirectX11Core() {}
};
