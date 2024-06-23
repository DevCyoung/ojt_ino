#include "pch.h"
#include "InnoSimulator.h"
#include "InnoDataManager.h"
#include <InputManager.h>
#define gLogListUIClient (static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient")))


static bool compare(const Vector3& a, const Vector3& b)
{
	return a.x < b.x;
}

InnoSimulator::InnoSimulator()
	: mState(eInnoSimulatorState::None)
	, mCurTime(0.f)
	, mMS(363.5f)
	, mMU(37.6f)
	, mKS(10300.0f)
	, mCS(1000.0f)
	, mKT(18000.0f)
	, mSpeed(1.0f)
	, mBumpStart(0.0f)
	, mBumpEnd(3.0f)
	, mBumpAmp(0.05f)
	, mSamplingTime(0.001f)
	, mPrevAPos(0.f)
	, mX{ 0.f }
	, mXDot{ 0.f }
	, mPlayerBPos(0.f)
	, mStartPos(0.f)
	, mFrameDeltaTime(0.f)
	, mBumps()
	, mBumpsCopy()
	, mPrevBPos(0.f)
{
}

InnoSimulator::~InnoSimulator()
{
}

void InnoSimulator::start()
{
}

void InnoSimulator::playing()
{
}

void InnoSimulator::finish()
{
	mPlayerBPos = 0.f;
	InnoDataManager::GetInstance()->Clear();
}

void InnoSimulator::Play()
{
	if (mState == eInnoSimulatorState::None)
	{
		mState = eInnoSimulatorState::Start;
	}
	else
	{
		Assert(false, ASSERT_MSG_INVALID);
	}
}

void InnoSimulator::Stop()
{
	if (mState == eInnoSimulatorState::Playing)
	{
		mState = eInnoSimulatorState::Stop;
	}
	else if (mState == eInnoSimulatorState::Editing)
	{
		mState = eInnoSimulatorState::Stop;
	}
	else
	{
		Assert(false, ASSERT_MSG_INVALID);
	}
}

void InnoSimulator::ServerStop()
{
	if (mState != eInnoSimulatorState::None)
	{
		mState = eInnoSimulatorState::Stop;
	}
}

void InnoSimulator::ServerStart()
{
	mState = eInnoSimulatorState::Start;
}


void InnoSimulator::Finish()
{	
	if (mState == eInnoSimulatorState::Editing)
	{
		mState = eInnoSimulatorState::Finish;
	}
	else
	{
		Assert(false, ASSERT_MSG_INVALID);
	}
}

void InnoSimulator::Update()
{
	if (mState == eInnoSimulatorState::None)
	{
		return;
	}
	else if (mState == eInnoSimulatorState::Start)
	{
		gLogListUIClient->WriteLine("Training Start");		
		InnoDataManager::GetInstance()->Clear();

		mPlayerBPos = 0.f;
		mPrevBPos = 0.f;

		mCurTime = 0.f;
		mPrevAPos = mStartPos;

		mFrameDeltaTime = 0.f;

		//Start, End 앞뒤조정 범프정렬
		sort();

		mBumpsCopy = mBumps;

		ZeroMemory(mX, sizeof(mX));
		ZeroMemory(mXDot, sizeof(mXDot));

		mState = eInnoSimulatorState::Playing;
	}
	else if (mState == eInnoSimulatorState::Playing)
	{	
		float speed = GetSpeed();

		if (gInput->GetKey(eKeyCode::SPACE))
		{
			speed += 3 * gDeltaTime;
			SetSpeed(speed);
		}
		else
		{		
			speed -= 3 * gDeltaTime;
			SetSpeed(speed);
		}
		

		mCurTime += gDeltaTime;	
		mFrameDeltaTime += gDeltaTime;

		if (mFrameDeltaTime < INNO_FRAME_DELTA_TIME)
		{
			return;
		}
	
		InnoDataManager::GetInstance()->PushPlayerASampleData(CreateSampleData(mCurTime, mFrameDeltaTime));
		mFrameDeltaTime = 0.f;
	}
	else if (mState == eInnoSimulatorState::Stop)
	{	
		gLogListUIClient->WriteLine("Training Stop");
		mState = eInnoSimulatorState::Editing;		
	}	
	else if (mState == eInnoSimulatorState::Finish)
	{
		gLogListUIClient->WriteLine("Editing Stop");
		gLogListUIClient->WriteLine("Training Finish");
		finish();
		mState = eInnoSimulatorState::None;		
	}
}

template<typename T>
T Innolerp(const T& a, const T& b, float t) 
{
	T ret = a + (b - a) * t;
	if (ret < a)
	{
		ret = a;
	}
	if (ret > b)
	{
		ret = b;
	}
	return ret;
}

tInnoSampleData InnoSimulator::CreateSampleData(float sampleTime, float deltaTime)
{
	//mSamplingTime 이전프레임과 지금프레임의 거리
	//도로의 높이
	const float curPos = mPrevAPos + mSpeed * deltaTime;
	float curOtherPos = mPrevBPos + mPlayerBSpeed * deltaTime;

	const float distance = abs(abs(mPlayerBPos) - abs(curOtherPos));

	if (distance > 1.f)
	{
		//curOtherPos가 mPlayerBPos로 보간
		curOtherPos = Innolerp(curOtherPos, mPlayerBPos, deltaTime);
	}

	float zr = 0.f;	
	if (!mBumpsCopy.empty())
	{
		const float bumpStart = mBumpsCopy[0].x;
		const float bumpEnd = mBumpsCopy[0].y;
		const float bumpAmp = mBumpsCopy[0].z;

		const float t1 = bumpStart / mSpeed;
		const float t2 = bumpEnd / mSpeed;

		//Road 체크 변경
		if (bumpStart <= curPos && curPos < bumpEnd)
		{
			zr = bumpAmp * sin(2.0f * XM_PI / (bumpEnd - bumpStart) / 2.0f * (curPos - bumpStart));
		}
		
		//Bump제거
		if (curPos >= bumpEnd)
		{
			mBumpsCopy.erase(mBumpsCopy.begin());
		}
	}		

	mXDot[0] = mX[1];
	mXDot[1] = (-mKS * (mX[0] - mX[2]) - mCS * (mX[1] - mX[3])) / mMS;
	mXDot[2] = mX[3];
	mXDot[3] = (mKS * (mX[0] - mX[2]) + mCS * (mX[1] - mX[3]) - mKT * (mX[2] - zr)) / mMU;

	//적분
	mX[0] = mX[0] + deltaTime * mXDot[0];
	mX[1] = mX[1] + deltaTime * mXDot[1];
	mX[2] = mX[2] + deltaTime * mXDot[2];
	mX[3] = mX[3] + deltaTime * mXDot[3];

	tInnoSampleData sampleData = {};

	sampleData.Time = sampleTime;
	sampleData.ZsPos = mX[0];
	sampleData.ZsSpeed = mX[1];
	sampleData.ZsAcc = mXDot[1];
	sampleData.ZuPos = mX[2];
	sampleData.ZuSpeed = mX[3];
	sampleData.ZuAcc = mXDot[3];
	sampleData.Zr = zr;
	sampleData.xSpeed = mSpeed;
	sampleData.xPos = curPos;
	sampleData.xPosOther = curOtherPos;// mPlayerBPos;

	mPrevAPos = sampleData.xPos;
	mPrevBPos = sampleData.xPosOther;
	return sampleData;
}

void InnoSimulator::PushBump(Vector3 bump)
{
	mBumps.push_back(bump);
}

void InnoSimulator::RemoveBump(int idx)
{
	mBumps.erase(mBumps.begin() + idx);
}

void InnoSimulator::SetBump(int idx, Vector3 bump)
{
	mBumps[idx] = bump;
}

void InnoSimulator::sort()
{
	for (int i = 0; i < mBumps.size(); ++i)
	{
		if (mBumps[i].x > mBumps[i].y)
		{
			float temp = mBumps[i].y;
			mBumps[i].y = mBumps[i].x;
			mBumps[i].x = temp;
		}
	}

	std::sort(mBumps.begin(), mBumps.end(), compare);
}
