#pragma once
#include <memory>
#include <assert.h>
//#include <Helper\Helper.h>

class GraphicDeviceDX11;
class RenderTargetRenderer;

class Engine
{
	friend int APIENTRY::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow);
	SINGLETON_DECLARE(Engine);

private:
	Engine(const HWND hWnd, const UINT renderTargetWidth, const UINT renderTargetHeight);

public:
	Vector2 GetRenderTargetSize() const  
	{  
		return Vector2(static_cast<float>(mRenderTargetWidth), static_cast<float>(mRenderTargetHeight));
	}

	Vector2 GetWindowScreenSize() const
	{
		return Vector2(static_cast<float>(mWindowScreenWidth), static_cast<float>(mWindowScreenHeight));
	}

	HWND GetHwnd() const { Assert(mHwnd, ASSERT_MSG_NULL); return mHwnd; }
	GraphicDeviceDX11* GetGraphicDevice() const { return mGraphicDevice; }

	void OmSet();

private:
	static void initialize(const HWND hWnd, const UINT renderTargetWidth, const UINT renderTargetHeight);
	void setWindowSize(const UINT windowScreenWidth, const UINT windowScreenHeight);
	void run();
	void updateWindowInfo();
	void update();
	void lateUpdate();
	void render();
	void eventUpdate();
	void present();
private:
	HWND mHwnd;
	UINT mRenderTargetWidth;
	UINT mRenderTargetHeight;
	UINT mWindowScreenWidth;
	UINT mWindowScreenHeight;

	GraphicDeviceDX11* mGraphicDevice;	
};

#define gEngine Engine::GetInstance()
#define gGraphicDevice gEngine->GetGraphicDevice()