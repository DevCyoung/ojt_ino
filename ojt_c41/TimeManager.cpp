#include "pch.h"
#include "TimeManager.h"
#include "MessageManager.h"

#include "InputManager.h"
TimeManager::TimeManager()
	: mDeltaTime(0.0f)
	, mGlobalTime(0.0f)
	, mRealDeltaTime(0.0f)
	, mSecond(0.0f)
	, mTimeScale(1.0f)
	, mCpuFrequency{}
	, mPrevFrequency{}
	, mCurFrequency{}
	, mbReset(false)
{
	QueryPerformanceFrequency(&mCpuFrequency);
	QueryPerformanceCounter(&mPrevFrequency);
}

TimeManager::~TimeManager()
{
}

void TimeManager::update()
{
	QueryPerformanceCounter(&mCurFrequency);

	const float DIFERENCE_FREQUENCY = static_cast<float>(mCurFrequency.QuadPart - mPrevFrequency.QuadPart);

	mDeltaTime = DIFERENCE_FREQUENCY / static_cast<float>(mCpuFrequency.QuadPart);

	if (mDeltaTime >= 0.026f)
	{
		mDeltaTime = 0.016f;
	}

	mRealDeltaTime = mDeltaTime;

	mDeltaTime *= mTimeScale;
	mGlobalTime += mDeltaTime;
	mPrevFrequency.QuadPart = mCurFrequency.QuadPart;

	if (MessageManager::GetInstance()->IsAddTitleMessage())
	{
		constexpr UINT FPS_LEN = 256;
		const float FPS = 1.0f / mDeltaTime;		
		wchar_t buffer[FPS_LEN] = { 0, };

		swprintf_s(buffer, FPS_LEN, L"<FPS : %.f>", FPS);

		MessageManager::GetInstance()->AddTitleMessage(buffer);
	}
}

void TimeManager::StartTime(LARGE_INTEGER* const starTime)
{
	QueryPerformanceCounter(starTime);
}

float TimeManager::EndTime(LARGE_INTEGER* const starTime)
{
	LARGE_INTEGER endTime;

	QueryPerformanceCounter(&endTime);

	const float DIFERENCE_FREQUENCY = static_cast<float>(endTime.QuadPart - starTime->QuadPart);

	return DIFERENCE_FREQUENCY / static_cast<float>(mCpuFrequency.QuadPart);
}

void TimeManager::ResetTime()
{
	mbReset = true;	

	mGlobalTime = 0.0f;
	mDeltaTime = 0.0f;
	mRealDeltaTime = 0.0f;
	mSecond = 0.0f;
	mTimeScale = 1.0f;
}
