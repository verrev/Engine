#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Input
{
public:
	static bool initialize(const HWND &window, const HINSTANCE &instance);
	static void update();
	
	inline static bool Input::isKeyDown(const char &key)
	{
		return (mKeyboardState[key] & 0x80) != 0;
	}
	inline static bool Input::isMouseButtonDown(const int &button)
	{
		return (mMouseState.rgbButtons[button] & 0x80) != 0;
	}
	inline static float Input::getMouseX()
	{
		return (float)mMouseState.lX;
	}
	inline static float Input::getMouseY()
	{
		return (float)mMouseState.lY;
	}
	inline static float Input::getMouseZ()
	{
		return (float)mMouseState.lZ;
	}
	static void destroy();
private:
	static IDirectInput8 *mDInput;
	static IDirectInputDevice8 *mKeyboard;
	static IDirectInputDevice8 *mMouse;
	static char mKeyboardState[256];
	static DIMOUSESTATE2 mMouseState;
private:
	Input() {}
};
