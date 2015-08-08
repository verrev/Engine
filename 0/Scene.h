#pragma once
#include "Model.h"
#include "Camera.h"
#include "Input.h"
#include "Sky.h"
#include "Timer.h"

#define FS 0

struct CBLight_PS
{
	XMFLOAT3 gAmbient;
	float pad0;

	XMFLOAT3 gDiffuse;
	float pad1;

	XMFLOAT3 gSpecular;
	float pad2;

	XMFLOAT3 gLightDir;
	float pad3;

	XMFLOAT3 gEyePos;
	float pad4;
};


class Scene
{
public:
	void initialize(const UINT &w, const UINT &h, const HWND &window, const HINSTANCE &inst);
	void render();
	void destroy();
private:
	Timer t;
	Sky s;
	Model terrain;
	Model box;
	Model sphere;
	Model car;
	Model gun;
	Model house;
	Model tree;
	std::vector<Model> bullets;

	CBLight_PS cbl_PS;
	ID3D11Buffer *light_PS;

	Camera *c;
};