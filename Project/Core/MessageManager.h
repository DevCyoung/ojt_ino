#pragma once
//#include "Singleton.h"

class MessageManager
{
	friend class Engine;
	SINGLETON_DECLARE(MessageManager);

public:
	void AddTitleMessage(const std::wstring& message)
	{
		Assert(IsAddTitleMessage(), ASSERT_MSG_INVALID);

		mTitleMessages.push_back(message);
	}

	bool IsAddTitleMessage() const { return mTimeInterval > 1.0f; }

private:
	void eventUpdate(const HWND hWnd);

private:
	std::vector<std::wstring> mTitleMessages;
	float mTimeInterval;
};
#define gMessageManager MessageManager::GetInstance()
