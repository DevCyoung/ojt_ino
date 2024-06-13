#include "pch.h"
#include "InnoDataManager.h"

InnoDataManager::InnoDataManager()	
	: mTimes(1200.f, 0.f)
	, mZsPoses(1200.f, 0.f)
	, mZsSpeeds(1200.f, 0.f)
	, mZsAccs(1200.f, 0.f)
	, mZuPoses(1200.f, 0.f)
	, mZuSpeeds(1200.f, 0.f)
	, mZuAccs(1200.f, 0.f)
	, mZrs(1200.f, 0.f)
	, mXPoses(1200.f, 0.f)
	, mXSpeeds(1200.f, 0.f)
	, mPlayerBSamplePostions(1200.f, 0.f)
{

	mTimes.reserve(1000000);

	mZsPoses.reserve(1000000);
	mZsSpeeds.reserve(1000000);
	mZsAccs.reserve(1000000);

	mZuPoses.reserve(1000000);
	mZuSpeeds.reserve(1000000);
	mZuAccs.reserve(1000000);

	mZrs.reserve(1000000);

	mXPoses.reserve(1000000);
	mXSpeeds.reserve(1000000);

	mPlayerBSamplePostions.reserve(1000000);
}

InnoDataManager::~InnoDataManager()
{
}

void InnoDataManager::PushPlayerASampleData(const tInnoSampleData& sampleData)
{
	mTimes.push_back(sampleData.Time);

	mZsPoses.push_back(sampleData.ZsPos);	
	mZsSpeeds.push_back(sampleData.ZsSpeed);
	mZsAccs.push_back(sampleData.ZsAcc);

	mZuPoses.push_back(sampleData.ZuPos);
	mZuSpeeds.push_back(sampleData.ZuSpeed);
	mZuAccs.push_back(sampleData.ZuAcc);

	mZrs.push_back(sampleData.Zr);

	mXPoses.push_back(sampleData.xPos);
	mXSpeeds.push_back(sampleData.xSpeed);

	mPlayerBSamplePostions.push_back(sampleData.xPosOther);
}
