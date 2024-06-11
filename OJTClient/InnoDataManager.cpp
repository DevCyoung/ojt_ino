#include "pch.h"
#include "InnoDataManager.h"

InnoDataManager::InnoDataManager()
	: mPlayerAPos(0.f)
	, mPlayerBPos(0.f)
	, mPlayerASampleDatas(0.f)
	, mPlayerBSamplePostions(0.f)
{
	mPlayerASampleDatas.reserve(100000);
	mPlayerBSamplePostions.reserve(100000);
}

InnoDataManager::~InnoDataManager()
{
}
	