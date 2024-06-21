#pragma once
class Engine
{
	friend int APIENTRY::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow);
	PUBLIC_SINGLETON_DECLARE(Engine);

private:
	Engine(const HWND hWnd, const UINT renderTargetWidth, const UINT renderTargetHeight);

public:
	HWND GetHwnd() const { Assert(mHwnd, ASSERT_MSG_NULL); return mHwnd; }
	void setWindowSize(const UINT windowScreenWidth, const UINT windowScreenHeight);

private:
	static void initialize(const HWND hWnd, const UINT renderTargetWidth, const UINT renderTargetHeight);
	void run();
	void updateWindowInfo();
	void update();
	void eventUpdate();

public:
	HWND mHwnd;
	UINT mRenderTargetWidth;
	UINT mRenderTargetHeight;
	UINT mWindowScreenWidth;
	UINT mWindowScreenHeight;
};

#define gEngine Engine::GetInstance()
#define gGraphicDevice gEngine->GetGraphicDevice()