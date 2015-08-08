#include "Scene.h"	


void Scene::initialize(const UINT &w, const UINT &h, const HWND &window, const HINSTANCE &inst)
{
#ifdef FS
	DirectX11Core::initialize(w, h, window, 0);
#else
	DirectX11Core::initialize(w, h, window, 1);
#endif


	t.initialize();
	s.initialize();

	terrain.initialize("res/terrain.aaa");

	box.initialize("res/crate.aaa");
	box.setWorldMatrix(XMMatrixTranslation(-2.0f,0.5f,0.0f));
	sphere.initialize("res/teapot.aaa");
	sphere.setWorldMatrix(XMMatrixRotationX(-XM_PIDIV2) * XMMatrixTranslation(5.0f, 0.0f, 0.0f));

	car.initialize("res/car.aaa");
	gun.initialize("res/gun.aaa");
	house.initialize("res/house.aaa");
	house.setWorldMatrix(XMMatrixScaling(0.1f,0.1f,0.1f)*XMMatrixRotationX(-XM_PIDIV2)*XMMatrixTranslation(-15, 0.01f, 0));

	tree.initialize("res/tree.aaa");
	tree.setWorldMatrix(XMMatrixScaling(0.01f,0.01f,0.01f)*XMMatrixRotationX(-XM_PIDIV2)*XMMatrixTranslation(-5, 0, 0));

	terrain.setWorldMatrix(XMMatrixRotationX(XM_PIDIV2));

	light_PS = DirectX11Core::createConstantBuffer(sizeof(CBLight_PS), 1, 0, &cbl_PS);

	c = new FirstPersonCamera;
	c->initialize(DirectX11Core::mWindowWidth, DirectX11Core::mWindowHeight);

	Input::initialize(window, inst);
}
void Scene::render()
{
	t.begin();

	DirectX11Core::clearRenderTargetViews();

	terrain.draw();
	box.draw();
	sphere.draw();
	car.draw();
	tree.draw();

	gun.setWorldMatrix(XMMatrixScaling(0.0004f, 0.0004f, 0.0004f) *XMMatrixRotationY(-XM_PIDIV2 - 0.1f)*XMMatrixTranslation(0.06f,-0.05,0.2f)* XMMatrixInverse(nullptr, c->getViewMatrix()));
	gun.draw();
	house.draw();
	static float f = 0.0f; f += 0.001f;

	Input::update();
	
	cbl_PS.gAmbient = XMFLOAT3(0.5, 0.5, 0.5);
	cbl_PS.gDiffuse = XMFLOAT3(0.2, 0.2, 0.2);
	cbl_PS.gSpecular = XMFLOAT3(0.2, 0.2, 0.2);
	cbl_PS.gLightDir = XMFLOAT3(1, -0, 1);
	cbl_PS.gEyePos = c->getPos();
	
	XMFLOAT3 p = XMFLOAT3(0,0,0);
	if (Input::isMouseButtonDown(0) && bullets.size() < 10) {
		Model bullet;
		bullet.initialize("res/sphere.aaa");
		bullet.setWorldMatrix(XMMatrixScaling(0.1, 0.1, 0.1)* XMMatrixTranslation(0, 0, 1)*XMMatrixInverse(nullptr, c->getViewMatrix()));
		bullets.push_back(bullet);
		p = c->getTarget();

	}

	DirectX11Core::mDeviceContext->UpdateSubresource(light_PS, 0, 0, &cbl_PS, 0, 0);
	DirectX11Core::mDeviceContext->PSSetConstantBuffers(1, 1, &light_PS);

	float lr = 0, bf = 0, dt = t.getDelta(), factor = 4.5f;
	if (Input::isKeyDown(DIK_W)) {
		bf = dt * factor;
	}
	else if (Input::isKeyDown(DIK_S)) {
		bf = -dt * factor;
	}
	if (Input::isKeyDown(DIK_A)) {
		lr = -dt * factor;
	}
	else if (Input::isKeyDown(DIK_D)) {
		lr = dt * factor;
	}

	static float fff = 0; fff += 0.1f;
	for (auto a : bullets) {
		//a.setWorldMatrix(XMMatrixScaling(0.1, 0.1, 0.1)* XMMatrixTranslation(0, 0, fff)*XMMatrixInverse(nullptr, c->getViewMatrix()));
		
		a.setWorldMatrix(a.getWorldMatrix() * XMMatrixTranslation(p.x*fff, p.y*fff, p.z*fff));
		a.draw();
		if (fff > 15) {
			fff = 0;
			bullets.pop_back();
			break;
		}
	}

	static float yaw = 0, pitch = 0, mouseFactor = 1.2f;
	yaw += Input::getMouseX() * dt * mouseFactor;
	pitch += Input::getMouseY() * dt * mouseFactor;
	c->update(lr, bf, yaw, pitch);

	s.draw(c->getPos());

	DirectX11Core::endScene();
	t.end(1);

	FrameStats fs = t.getStats();
	std::wstring s = L"FPS: ";
	s.append(std::to_wstring((int)fs.fps));
	s.append(L" frametime: ");
	s.append(std::to_wstring(fs.msPerFrame));
	s.append(L" (ms)");
	SetWindowText(DirectX11Core::mWindow, s.c_str());
}
void Scene::destroy()
{
	delete c;
	Input::destroy();
	DirectX11Core::destroy();
}