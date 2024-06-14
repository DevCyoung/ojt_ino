#include "pch.h"
#include "InnoDataManager.h"

InnoDataManager::InnoDataManager()	
	: mTimes(INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND, 0.f)
	, mZsPoses(INNO_CLIENT_FRAME_PER_SECOND* INNO_GRAPH_HISTORY_SECOND, 0.f)
	, mZsSpeeds(INNO_CLIENT_FRAME_PER_SECOND* INNO_GRAPH_HISTORY_SECOND, 0.f)
	, mZsAccs(INNO_CLIENT_FRAME_PER_SECOND* INNO_GRAPH_HISTORY_SECOND, 0.f)
	, mZuPoses(INNO_CLIENT_FRAME_PER_SECOND* INNO_GRAPH_HISTORY_SECOND, 0.f)
	, mZuSpeeds(INNO_CLIENT_FRAME_PER_SECOND* INNO_GRAPH_HISTORY_SECOND, 0.f)
	, mZuAccs(INNO_CLIENT_FRAME_PER_SECOND* INNO_GRAPH_HISTORY_SECOND, 0.f)
	, mZrs(INNO_CLIENT_FRAME_PER_SECOND* INNO_GRAPH_HISTORY_SECOND, 0.f)
	, mXPoses(INNO_CLIENT_FRAME_PER_SECOND* INNO_GRAPH_HISTORY_SECOND, 0.f)
	, mXSpeeds(INNO_CLIENT_FRAME_PER_SECOND* INNO_GRAPH_HISTORY_SECOND, 0.f)
	, mPlayerBSamplePostions(INNO_CLIENT_FRAME_PER_SECOND* INNO_GRAPH_HISTORY_SECOND, 0.f)
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

void InnoDataManager::Clear()
{
	mTimes.clear();
	mZsPoses.clear();
	mZsSpeeds.clear();
	mZsAccs.clear();
	mZuPoses.clear();
	mZuSpeeds.clear();
	mZuAccs.clear();
	mZrs.clear();
	mXPoses.clear();
	mXSpeeds.clear();
	mPlayerBSamplePostions.clear();

	for (int i = 0; i < INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND; ++i)
	{
		mTimes.push_back(0.f);
		mZsPoses.push_back(0.f);
		mZsSpeeds.push_back(0.f);
		mZsAccs.push_back(0.f);
		mZuPoses.push_back(0.f);
		mZuSpeeds.push_back(0.f);
		mZuAccs.push_back(0.f);
		mZrs.push_back(0.f);
		mXPoses.push_back(0.f);
		mXSpeeds.push_back(0.f);
		mPlayerBSamplePostions.push_back(0.f);
	}
}
