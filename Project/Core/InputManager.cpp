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
	, mMousePos(Vector2::Zero)
	, mPrevMousePos(Vector2::Zero)
	, mMouseDir(Vector2::Zero)
	, mMonitorSize(Vector2(3840,1080))
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

bool InputManager::IsWindowMouseHoverd()
{
	const Vector2& WINDOW_SCREEN_SIZE = gEngine->GetWindowScreenSize();

	return (0.f <= mMousePos.x && mMousePos.x <= WINDOW_SCREEN_SIZE.x) &&
		(0.f <= mMousePos.y && mMousePos.y <= WINDOW_SCREEN_SIZE.y);
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
		//ScreenToClient(hWnd, &ptMousePos);
		ScreenToClient(focusWindow, &ptScreenMousePos);
		if (gInput->GetKeyDown(eKeyCode::Q))
		{
			//ImVec2 cursurPos = ImGui::GetCursorPos();
			int a = 3;
			(void)a;
		}
		mMousePos.x = static_cast<float>(ptScreenMousePos.x);
		mMousePos.y = static_cast<float>(ptScreenMousePos.y);		

		mMouseDir = mMousePos - mPrevMousePos;

		//마우스가 끝에있을때
		if (ptMousePos.x <= 8)
		{
			mMouseDir.x = -1.f;
		}
		else if (ptMousePos.x >= mMonitorSize.x / 2 - 8)
		{
			mMouseDir.x = 1.f;
		}
		if (ptMousePos.y <= 8)
		{
			mMouseDir.y = -1.f;
		}
		else if (ptMousePos.y >= mMonitorSize.y - 8)
		{
			mMouseDir.y = 1.f;
		}

		mMouseDir.y *= -1;

		mPrevMousePos = mMousePos;

		if (false == IsWindowMouseHoverd())
		{
			if (GetKeyState(eKeyCode::LBTN) != eKeyState::None)
			{
				mKeyInfos[static_cast<UINT>(eKeyCode::LBTN)].bPressed = false;
				mKeyInfos[static_cast<UINT>(eKeyCode::LBTN)].state = eKeyState::None;
			}
			if (GetKeyState(eKeyCode::RBTN) != eKeyState::None)
			{
				mKeyInfos[static_cast<UINT>(eKeyCode::RBTN)].bPressed = false;
				mKeyInfos[static_cast<UINT>(eKeyCode::RBTN)].state = eKeyState::None;
			}
		}
		
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
