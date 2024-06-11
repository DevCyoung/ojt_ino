#pragma once
#include "CarSimulation.h"

class InnoDataManager
{
	PUBLIC_SINGLETON_DECLARE(InnoDataManager);

public:
	//시뮬레이션중 실시간 포지션
	void SetPlayerAPos(float playerAPos) { mPlayerAPos = playerAPos; }
	void SetPlayerBPos(float playerBPos) { mPlayerBPos = playerBPos; }
	float GetPlayerPosA() { return mPlayerAPos; }
	float GetPlayerPosB() { return mPlayerBPos; }

	//단위시간마다 SampleDadta를 할당
	void PushPlayerASampleData(const tSampleData& sampleData) { mPlayerASampleDatas.push_back(sampleData); }

	//마지막에 데이터교환할때 할당
	void PushPlayerBSampleDataPosition(const float sampleDataPosition) { mPlayerBSamplePostions.push_back(sampleDataPosition); }

	//시뮬레이션 종료후에 필요한 데이터들
	const std::vector<tSampleData>& GetPlayerASampleDatas() { return mPlayerASampleDatas; }
	const std::vector<float>&		GetPlayerBSamplePositions() { return mPlayerBSamplePostions; }

private:
	float mPlayerAPos;
	float mPlayerBPos;

	std::vector<tSampleData> mPlayerASampleDatas;
	std::vector<float> mPlayerBSamplePostions;
};

