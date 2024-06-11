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
	, mSpeed(10.0f)
	, mBumpStart(0.0f)
	, mBumpEnd(3.0f)
	, mBumpAmp(0.05f)
	, mSamplingTime(0.001f)
	, mPrevPos(0.f)
	, mX{0.f}
	, mXDot{ 0.f }
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
}

void InnoSimulator::Play()
{
	Assert(mState == eInnoSimulatorState::None, ASSERT_MSG_INVALID);


	if (mState == eInnoSimulatorState::None)
	{
		mState = eInnoSimulatorState::Start;
	}
}

void InnoSimulator::Finish()
{
	if (mState == eInnoSimulatorState::None)
	{
		mState = eInnoSimulatorState::Finish;
	}
}

void InnoSimulator::Update()
{
	if (mState == eInnoSimulatorState::None)
	{
		return;
	}
	else if (mState == eInnoSimulatorState::Finish)
	{
		finish();
		mState = eInnoSimulatorState::None;
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
		mCurTime += gDeltaTime;

		InnoDataManager* dataManager = InnoDataManager::GetInstance();
		tInnoSampleData sampleData = CreateSampleData(mCurTime);
		dataManager->PushPlayerASampleData(sampleData);
	}
}

tInnoSampleData InnoSimulator::CreateSampleData(float sampleTime)
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
	mX[0] = mX[0] + gDeltaTime * mXDot[0];
	mX[1] = mX[1] + gDeltaTime * mXDot[1];
	mX[2] = mX[2] + gDeltaTime * mXDot[2];
	mX[3] = mX[3] + gDeltaTime * mXDot[3];

	tInnoSampleData sampleData = {};	

	sampleData.Time		= sampleTime;
	sampleData.ZsPos	= mX[0];
	sampleData.ZsSpeed	= mX[1];
	sampleData.ZsAcc	= mXDot[1];
	sampleData.ZuPos	= mX[2];
	sampleData.ZuSpeed	= mX[3];
	sampleData.ZuAcc	= mXDot[3];
	sampleData.Zr		= zr;
	sampleData.xPos		= mPrevPos + mSpeed * gDeltaTime;
	sampleData.xSpeed	= mSpeed;


	mPrevPos = sampleData.xPos;
	return sampleData;
}
