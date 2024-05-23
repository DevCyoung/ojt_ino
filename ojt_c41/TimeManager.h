#pragma once

class TimeManager
{
	friend class Engine;	
	friend class Content;
	friend class Scene;

	SINGLETON_DECLARE(TimeManager);

private:
	void update();	

public:
	float GetDeltaTime() const { return mDeltaTime; }
	float GetGlobalTime() const { return mGlobalTime; }
	float GetRealDeltaTime() const { return mRealDeltaTime; }

	void StartTime(LARGE_INTEGER* const starTime);
	float EndTime(LARGE_INTEGER* const starTime);

	float GetTileScale() const { return mTimeScale; }
	void SetTileScale(const float tileScale) { mTimeScale = tileScale; }

	void ResetTime();

private:
	float mDeltaTime;
	float mRealDeltaTime;
	float mGlobalTime;
	float mSecond;
	float mTimeScale;

	LARGE_INTEGER mCpuFrequency;
	LARGE_INTEGER mPrevFrequency;
	LARGE_INTEGER mCurFrequency;

	bool mbReset;
};

#define gDeltaTime TimeManager::GetInstance()->GetDeltaTime()
#define gGlobalTime TimeManager::GetInstance()->GetGlobalTime()
#define gRealDeltaTime TimeManager::GetInstance()->GetRealDeltaTime()