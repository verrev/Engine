#include <vld.h>
#include "Scene.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = L"wc1";
	wc.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx(&wc);
	ShowCursor(0);
#ifdef FS
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
#else
	int w = 800, h = w;
#endif
	HWND gameWindow = CreateWindowEx(0, L"wc1", L"0", WS_VISIBLE | WS_BORDER, 0, 0, w, h, 0, 0, hInstance, 0);
	Scene s;
	s.initialize(w, h, gameWindow, hInstance);

	MSG msg;
	while (!GetAsyncKeyState(VK_ESCAPE)) {
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		s.render();
	}
	s.destroy();
	return 0;
}