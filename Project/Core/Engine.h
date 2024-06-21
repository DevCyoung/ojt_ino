#pragma once
class Engine
{
	friend int APIENTRY::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow);
	SINGLETON_DECLARE(Engine);

private:
	Engine(const HWND hWnd, const UINT renderTargetWidth, const UINT renderTargetHeight);

public:
	HWND GetHwnd() const { Assert(mHwnd, ASSERT_MSG_NULL); return mHwnd; }

private:
	static void initialize(const HWND hWnd, const UINT renderTargetWidth, const UINT renderTargetHeight);
	void setWindowSize(const UINT windowScreenWidth, const UINT windowScreenHeight);
	void run();
	void updateWindowInfo();
	void update();
	void eventUpdate();

private:
	HWND mHwnd;
	UINT mRenderTargetWidth;
	UINT mRenderTargetHeight;
	UINT mWindowScreenWidth;
	UINT mWindowScreenHeight;
};

#define gEngine Engine::GetInstance()
#define gGraphicDevice gEngine->GetGraphicDevice()