#include "pch.h"
#include "InnoSimulator.h"
#include "InnoDataManager.h"
#include <TimeManager.h>

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
	, mPrevPos(0.f)
	, mX{ 0.f }
	, mXDot{ 0.f }
	, mPlayerBPos(0.f)
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
	else
	{
		Assert(false, ASSERT_MSG_INVALID);
	}
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
		start();
		//clear
		mCurTime = 0.f;
		mPrevPos = 0.f;
		mState = eInnoSimulatorState::Playing;
	}
	else if (mState == eInnoSimulatorState::Playing)
	{
		static float frameDeltatime = 0.f;
		mCurTime += gDeltaTime;
		frameDeltatime += gDeltaTime;

		if (frameDeltatime < INNO_FRAME_DELTA_TIME)
		{
			return;
		}

		InnoDataManager::GetInstance()->PushPlayerASampleData(CreateSampleData(mCurTime, frameDeltatime));
		frameDeltatime = 0.f;
	}
	else if (mState == eInnoSimulatorState::Stop)
	{		
		mState = eInnoSimulatorState::Editing;
	}
	else if (mState == eInnoSimulatorState::Editing)
	{
			
	}
	else if (mState == eInnoSimulatorState::Finish)
	{
		finish();
		mState = eInnoSimulatorState::None;
	}
}

tInnoSampleData InnoSimulator::CreateSampleData(float sampleTime, float deltaTime)
{
	//mSamplingTime 이전프레임과 지금프레임의 거리
	//도로의 높이
	float zr = 0.f;

	float t1 = mBumpStart / mSpeed;
	float t2 = mBumpEnd / mSpeed;

	if (t1 <= sampleTime && sampleTime < t2)
	{
		zr = -mBumpAmp * sin(2.0f * XM_PI / (t2 - t1) / 2.0f * (sampleTime - t1));
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
	sampleData.xPos = mPrevPos + mSpeed * deltaTime;
	sampleData.xSpeed = mSpeed;
	sampleData.xPosOther = mPlayerBPos;

	mPrevPos = sampleData.xPos;
	return sampleData;
}
