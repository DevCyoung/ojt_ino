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

	float x[4] = { 0, };
	float xdot[4] = { 0, };

	if (t1 <= sampleTime && sampleTime < t2)
	{
		zr = -mBumpAmp * sin(2.0f * XM_PI / (t2 - t1) / 2.0f * (sampleTime - t1));
	}

	xdot[0] = x[1];
	xdot[1] = (-mKS * (x[0] - x[2]) - mCS * (x[1] - x[3])) / mMS;
	xdot[2] = x[3];
	xdot[3] = (mKS * (x[0] - x[2]) + mCS * (x[1] - x[3]) - mKT * (x[2] - zr)) / mMU;

	//적분
	x[0] = x[0] + gDeltaTime * xdot[0];
	x[1] = x[1] + gDeltaTime * xdot[1];
	x[2] = x[2] + gDeltaTime * xdot[2];
	x[3] = x[3] + gDeltaTime * xdot[3];

	tInnoSampleData sampleData = {};	

	sampleData.Time		= sampleTime;
	sampleData.ZsPos	= x[0];
	sampleData.ZsSpeed	= x[1];
	sampleData.ZsAcc	= xdot[1];
	sampleData.ZuPos	= x[2];
	sampleData.ZuSpeed	= x[3];
	sampleData.ZuAcc	= xdot[3];
	sampleData.Zr		= zr;
	sampleData.xPos		= mPrevPos + mSpeed * gDeltaTime;
	sampleData.xSpeed	= mSpeed;


	mPrevPos = sampleData.xPos;
	return sampleData;
}
