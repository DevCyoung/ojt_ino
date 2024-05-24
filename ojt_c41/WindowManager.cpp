#include "pch.h"
#include "WindowManager.h"

WindowManager::WindowManager()	
	: mCurFocusHWND (NULL)
	, mPreFocusvHWND(NULL)
	, mWindowScreenPos(Vector2::Zero)
{
}

WindowManager::~WindowManager()
{
}

void WindowManager::update()
{
}
