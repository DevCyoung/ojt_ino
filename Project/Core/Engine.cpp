#include "pch.h"
#include "Engine.h"
#include "TimeManager.h"
#include "MessageManager.h"
#include "PathManager.h"
#include "InputManager.h"


Engine::Engine(const HWND hWnd, const UINT renderTargetWidth, const UINT renderTargetHeight)
	: mHwnd(hWnd)
	, mRenderTargetWidth(renderTargetWidth)
	, mRenderTargetHeight(renderTargetHeight)
	, mWindowScreenWidth(renderTargetWidth)
	, mWindowScreenHeight(renderTargetHeight)
{
	setWindowSize(mRenderTargetWidth, mRenderTargetHeight);
	InputManager::initialize();
	TimeManager::initialize();
	MessageManager::initialize();		
	PathManager::initialize();
}

Engine::~Engine()
{
	InputManager::deleteInstance();
	PathManager::deleteInstance();
	MessageManager::deleteInstance();	
	TimeManager::deleteInstance();		
}

void Engine::initialize(const HWND hWnd, const UINT renderTargetWidth, const UINT renderTargetHeight)
{
	Assert(hWnd, ASSERT_MSG_NULL);
	Assert(!sInstance, ASSERT_MSG_NOT_NULL);

	sInstance = new Engine(hWnd, renderTargetWidth, renderTargetHeight);
}

void Engine::run()
{
	updateWindowInfo();	

	update();

	eventUpdate();
}

void Engine::updateWindowInfo()
{
	Assert(mHwnd, ASSERT_MSG_NULL);

	RECT windowRect;
	GetClientRect(mHwnd, &windowRect);

	mWindowScreenWidth = windowRect.right - windowRect.left;
	mWindowScreenHeight = windowRect.bottom - windowRect.top;
}

void Engine::update()
{
	TimeManager::GetInstance()->update();
	InputManager::GetInstance()->update(mHwnd);
}

void Engine::eventUpdate()
{
	MessageManager::GetInstance()->eventUpdate(mHwnd);
}

void Engine::setWindowSize(const UINT windowScreenWidth, const UINT windowScreenHeight)
{
	Assert(mHwnd, ASSERT_MSG_NULL);

	RECT windowScreen =
	{
		0, 0,
		static_cast<LONG>(windowScreenWidth), static_cast<LONG>(windowScreenHeight)
	};

	AdjustWindowRect(&windowScreen, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, GetMenu(mHwnd) != nullptr);

	const int ADJUST_WIDTH = static_cast<int>(windowScreen.right - windowScreen.left);
	const int ADJUST_HEIGHT = static_cast<int>(windowScreen.bottom - windowScreen.top);

	const int LEFT_X_POS = GetSystemMetrics(SM_CXSCREEN) / 2 - static_cast<int>(ADJUST_WIDTH) / 2;
	const int LEFT_Y_POS = GetSystemMetrics(SM_CYSCREEN) / 2 - static_cast<int>(ADJUST_HEIGHT) / 2 - 17;

	SetWindowPos(mHwnd, nullptr,
		LEFT_X_POS, LEFT_Y_POS,
		ADJUST_WIDTH, ADJUST_HEIGHT, 0);
	ShowWindow(mHwnd, true);
	UpdateWindow(mHwnd);
}
