////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"


InputClass::InputClass()
{
	m_DirectxInput = 0;
	m_Keyboard = 0;
	m_Mouse = 0;
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}

bool InputClass::Initialize(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	m_ScreenHeight = screenHeight;
	m_ScreenWidth = screenWidth;

	//m_MouseX = m_ScreenHeight / 2;
	//m_MouseY = m_ScreenWidth / 2;

	m_MouseX = 0;
	m_MouseY = 0;

	result = DirectInput8Create(hInstance, DIRECTXINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectxInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_DirectxInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Keyboard->Acquire();
	if (FAILED(result))
	{
		//result = m_Keyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
		return false;
	}

	result = m_DirectxInput->CreateDevice(GUID_SysMouse, &m_Mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	result = m_Mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void InputClass::ShutDown()
{
	if (m_DirectxInput)
	{
		m_DirectxInput->Release();
		m_DirectxInput = 0;
	}

	if (m_Mouse)
	{
		m_Mouse->Unacquire();
		m_Mouse->Release();
		m_Mouse = 0;
	}

	if (m_Keyboard)
	{
		m_Keyboard->Unacquire();
		m_Keyboard->Release();
		m_Keyboard = 0;
	}
}

bool InputClass::Frame()
{
	bool result;

	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	ProcessInput();

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;
	result = m_Keyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_Keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;
	result = m_Mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_MouseState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_Mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputClass::ProcessInput()
{
	m_MouseX += m_MouseState.lX;
	m_MouseY += m_MouseState.lY;

	//m_MouseX = RANGE(m_MouseX, 0, m_ScreenWidth);
	//m_MouseY = RANGE(m_MouseY, 0, m_ScreenHeight);
}

void InputClass::GetMousePos(int& mouseX, int& mouseY)
{
	mouseX = m_MouseX;
	mouseY = m_MouseY;
}

bool InputClass::IsButtonClick(int key)
{
	return m_KeyboardState[key] & 0x80 ? true : false;
}

bool InputClass::IsButtonDown(int key)
{
	static int time = 0;
	if (m_KeyboardState[key] & 0x80)
	{
		time++;
		if (time == 1)
		{
			return true;
		}
		return false;
	}
	time = 0;
	return false;
}