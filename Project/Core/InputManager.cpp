#include "pch.h"
#include "InputManager.h"
#include "Engine.h"

#define SIGN_BIT 0x8000

constexpr static int INPUT_ASCII_TABLE[(UINT)eKeyCode::END] =
{
	VK_F1, VK_F2, VK_F3, VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10, VK_F11, VK_F12,
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',

	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M',

	VK_LSHIFT, VK_LCONTROL,

	VK_UP, VK_DOWN, VK_LEFT ,VK_RIGHT, VK_SPACE,

	VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,

	VK_LBUTTON, VK_RBUTTON
};

InputManager::InputManager()
	: mKeyInfos()
{
	mKeyInfos.reserve(static_cast<UINT>(eKeyCode::END));

	for (UINT i = 0; i < static_cast<UINT>(eKeyCode::END); ++i)
	{
		tKeyInfo keyInfo = {};

		keyInfo.key = static_cast<eKeyCode>(i);
		keyInfo.state = eKeyState::None;
		keyInfo.bPressed = false;
		mKeyInfos.push_back(keyInfo);
	}
}

InputManager::~InputManager()
{
}

void InputManager::update(const HWND hWnd)
{
	(void)(hWnd);
	Assert(hWnd, ASSERT_MSG_NULL);
	HWND focusWindow = GetFocus();

	if (focusWindow)
	{
		for (tKeyInfo& keyInfo : mKeyInfos)
		{
			const UINT INDEX = static_cast<UINT>(keyInfo.key);

			if (GetAsyncKeyState(INPUT_ASCII_TABLE[INDEX]) & SIGN_BIT)
			{
				if (keyInfo.bPressed == false)
				{
					keyInfo.state = eKeyState::Down;
					keyInfo.bPressed = true;
				}
				else
				{
					keyInfo.state = eKeyState::Pressed;
				}
			}
			else
			{
				if (keyInfo.bPressed == true)
				{
					keyInfo.state = eKeyState::Up;
					keyInfo.bPressed = false;
				}
				else
				{
					keyInfo.state = eKeyState::None;
				}
			}
		}

		POINT ptMousePos = {};
		GetCursorPos(&ptMousePos);
		POINT ptScreenMousePos = ptMousePos;
		ScreenToClient(focusWindow, &ptScreenMousePos);;
		

	}
	else
	{
		for (tKeyInfo& keyInfo : mKeyInfos)
		{
			switch (keyInfo.state)
			{
			case eKeyState::Down:
				[[fallthrough]];
			case eKeyState::Pressed:
				keyInfo.state = eKeyState::Up;
				keyInfo.bPressed = false;
				break;
			case eKeyState::Up:
				keyInfo.state = eKeyState::None;
				break;
			case eKeyState::None:
				break;
			default:
				Assert(false, ASSERT_MSG_INVALID);
				break;
			}
		}
	}
}