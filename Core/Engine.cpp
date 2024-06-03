#include "pch.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include "TimeManager.h"
#include "WindowManager.h"
#include "MessageManager.h"
#include "InputManager.h"
#include "PathManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "RenderTargetRenderer.h"
#include "EngineResourceLoader.h"
//#include "Fmod.h"
//#include "FontManager.h"
#include "BuildSetting.h"
#include "FBXLoadManager.h"
Engine::Engine(const HWND hWnd, const UINT renderTargetWidth, const UINT renderTargetHeight)
	: mHwnd(hWnd)
	, mRenderTargetWidth(renderTargetWidth)
	, mRenderTargetHeight(renderTargetHeight)
	, mWindowScreenWidth(renderTargetWidth)
	, mWindowScreenHeight(renderTargetHeight)
	, mGraphicDevice(new GraphicDeviceDX11(mHwnd, mRenderTargetWidth, mRenderTargetHeight))
{
	setWindowSize(mRenderTargetWidth, mRenderTargetHeight);
	FBXLoadManager::initialize();
	//Fmod::Initialize();	
	TimeManager::initialize();
	WindowManager::initialize();
	MessageManager::initialize();
	PathManager::initialize();
	InputManager::initialize();
	ResourceManager::initialize();
	SceneManager::initialize();
}

Engine::~Engine()
{
	SceneManager::deleteInstance();
	ResourceManager::deleteInstance();
	InputManager::deleteInstance();
	PathManager::deleteInstance();
	MessageManager::deleteInstance();
	WindowManager::deleteInstance();
	TimeManager::deleteInstance();	
	//FontManager::deleteInstance();
	//Fmod::Release();
	FBXLoadManager::deleteInstance();
	DELETE_POINTER_NOT_NULL(mGraphicDevice);	
}

void Engine::OmSet()
{
	const Vector2& RENDER_TARGET_SIZE = GetRenderTargetSize();

	const UINT RENDER_TARGET_WIDHTH = static_cast<UINT>(RENDER_TARGET_SIZE.x);
	const UINT RENDER_TARGET_HEIGHT = static_cast<UINT>(RENDER_TARGET_SIZE.y);

	gGraphicDevice->BindRenderTarget(RENDER_TARGET_WIDHTH,
		RENDER_TARGET_HEIGHT,
		1,
		gGraphicDevice->GetRenderTargetViewAddressOf(),
		gGraphicDevice->GetDepthStencilView());
}

void Engine::initialize(const HWND hWnd, const UINT renderTargetWidth, const UINT renderTargetHeight)
{
	Assert(hWnd, ASSERT_MSG_NULL);
	Assert(!sInstance, ASSERT_MSG_NOT_NULL);

	sInstance = new Engine(hWnd, renderTargetWidth, renderTargetHeight);

	//FontManager::initialize();
	EngineResourceLoader::loadResource();
}

void Engine::run()
{
	updateWindowInfo();	

	update();

	lateUpdate();

	render();

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
	SceneManager::GetInstance()->update();
}

void Engine::lateUpdate()
{
	SceneManager::GetInstance()->lateUpdate();
}

void Engine::render()
{	
#ifndef EDITOR_MODE
	SceneManager::GetInstance()->render(mRenderTargetWidth,
				mRenderTargetHeight,
				mGraphicDevice->GetRenderTargetViewAddressOf(),
				mGraphicDevice->GetDepthStencilView());			
	//FLOAT backgroundColor[4] = { 1.0f, 0.0f, 1.0f, 0.1f };
	//gGraphicDevice->ClearRenderTarget(
	//	mGraphicDevice->GetRenderTargetViewAddressOf(),
	//	mGraphicDevice->GetDepthStencilView(), backgroundColor);
#endif
}

void Engine::eventUpdate()
{
	SceneManager::GetInstance()->eventUpdate();
	MessageManager::GetInstance()->eventUpdate(mHwnd);
}

void Engine::present()
{
	mGraphicDevice->present();

	SceneManager::GetInstance()->flush();
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
