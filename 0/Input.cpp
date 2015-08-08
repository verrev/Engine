#include "Input.h"
#include "DirectX11Core.h"

IDirectInput8 *Input::mDInput = nullptr;
IDirectInputDevice8 *Input::mKeyboard = nullptr;
IDirectInputDevice8 *Input::mMouse = nullptr;
char Input::mKeyboardState[256];
DIMOUSESTATE2 Input::mMouseState;

bool Input::initialize(const HWND &window, const HINSTANCE &instance)
{
	ZeroMemory(&mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));

	HRESULT hr = DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDInput, 0);
	if (FAILED(hr)) {
		LOG("DirectInput8Create failed");
		return 0;
	}
	hr = mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0);
	if (FAILED(hr)) {
		LOG("DirectInput8Create failed");
		return 0;
	}
	hr = mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) {
		LOG("DirectInput8Create failed");
		return 0;
	}
	hr = mKeyboard->SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		LOG("DirectInput8Create failed");
		return 0;
	}
	hr = mKeyboard->Acquire();
	if (FAILED(hr)) {
		LOG("DirectInput8Create failed");
		return 0;
	}
	hr = mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0);
	if (FAILED(hr)) {
		LOG("DirectInput8Create failed");
		return 0;
	}
	hr = mMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) {
		LOG("DirectInput8Create failed");
		return 0;
	}
	hr = mMouse->SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		LOG("DirectInput8Create failed");
		return 0;
	}
	hr = mMouse->Acquire();
	if (FAILED(hr)) {
		LOG("DirectInput8Create failed");
		return 0;
	}
	return 1;
}
void Input::update()
{
	if (mKeyboard)
		mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState);
	else
		mKeyboard->Acquire();
	if (mMouse)
		mMouse->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&mMouseState);
	else
		mMouse->Acquire();
}
void Input::destroy()
{
	safeRelease(mDInput);
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	safeRelease(mKeyboard);
	safeRelease(mMouse);
}