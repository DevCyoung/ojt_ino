#include "pch.h"
#include "MessageManager.h"
#include "TimeManager.h"

MessageManager::MessageManager()
	: mTitleMessages()
	, mTimeInterval(0.0f)
{
}

MessageManager::~MessageManager()
{
}

void MessageManager::eventUpdate(const HWND hWnd)
{	
	if (IsAddTitleMessage())
	{
		constexpr UINT SHOW_TITLE_MESSAGE_MAX_LEN = 256;
		std::wstring showTitleMessage = L"";

		showTitleMessage.reserve(SHOW_TITLE_MESSAGE_MAX_LEN);
		//showTitleMessage = L"Katana Zero";

		for (const std::wstring& titleMessage : mTitleMessages)
		{
			showTitleMessage += L" ";
			showTitleMessage += titleMessage;
		}

		SetWindowText(hWnd, showTitleMessage.data());

		mTimeInterval = 0.0f;
		mTitleMessages.clear();
	}
	else
	{
		mTimeInterval += gDeltaTime;
	}	
}