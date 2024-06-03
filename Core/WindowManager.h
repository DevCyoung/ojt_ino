#pragma once
class WindowManager
{
	friend class Engine;
	SINGLETON_DECLARE(WindowManager);

public:
	void update();	
	void SetWindowScreenPos(const Vector2& windowScreenPos) { mWindowScreenPos = windowScreenPos; }
	Vector2 GetWindowMousePos() { return mWindowScreenPos; }

private:
	HWND mCurFocusHWND;
	HWND mPreFocusvHWND;
	Vector2 mWindowScreenPos;
};

#define gWindowManager PathManager::GetInstance()