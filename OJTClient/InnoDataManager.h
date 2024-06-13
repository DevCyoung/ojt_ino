#pragma once
#include "InnoSimulator.h"

class InnoDataManager
{
	PUBLIC_SINGLETON_DECLARE(InnoDataManager);

public:
	//�����ð����� SampleDadta�� �Ҵ�
	void PushPlayerASampleData(const tInnoSampleData& sampleData);

	//�������� �����ͱ�ȯ�Ҷ� �Ҵ�
	void PushPlayerBSampleDataPosition(const float sampleDataPosition) { mPlayerBSamplePostions.push_back(sampleDataPosition); }
	
	const std::vector<float>&		GetPlayerBSamplePositions() { return mPlayerBSamplePostions; }

	const std::vector<float>& GetTimes() { return mTimes; }
	const std::vector<float>& GetZsPoses() { return mZsPoses; }
	const std::vector<float>& GetZsSpeeds() { return mZsSpeeds; }
	const std::vector<float>& GeetZsAccs() { return mZsAccs; }
	const std::vector<float>& GetZuPoses() { return mZuPoses; }
	const std::vector<float>& GetZuSpeeds() { return mZuSpeeds; }
	const std::vector<float>& GetZuAccs() { return mZuAccs; }
	const std::vector<float>& GetZrs() { return mZrs; }
	const std::vector<float>& GetXPoses() { return mXPoses; }
	const std::vector<float>& GetXSpeeds() { return mXSpeeds; }
	const std::vector<float>& GetOtherPoses() { return mPlayerBSamplePostions; }

private:	
	std::vector<float> mTimes;
	std::vector<float> mZsPoses;
	std::vector<float> mZsSpeeds;
	std::vector<float> mZsAccs;
	std::vector<float> mZuPoses;
	std::vector<float> mZuSpeeds;
	std::vector<float> mZuAccs;
	std::vector<float> mZrs;
	std::vector<float> mXPoses;
	std::vector<float> mXSpeeds;
	std::vector<float> mPlayerBSamplePostions;
};

