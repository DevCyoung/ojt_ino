#pragma once
#include "InnoSimulator.h"

class InnoDataManager
{
	PUBLIC_SINGLETON_DECLARE(InnoDataManager);

public:
	//�ùķ��̼��� �ǽð� ������
	void SetPlayerAPos(float playerAPos) { mPlayerAPos = playerAPos; }
	void SetPlayerBPos(float playerBPos) { mPlayerBPos = playerBPos; }
	float GetPlayerPosA() { return mPlayerAPos; }
	float GetPlayerPosB() { return mPlayerBPos; }

	//�����ð����� SampleDadta�� �Ҵ�
	void PushPlayerASampleData(const tInnoSampleData& sampleData) { mPlayerASampleDatas.push_back(sampleData); }

	//�������� �����ͱ�ȯ�Ҷ� �Ҵ�
	void PushPlayerBSampleDataPosition(const float sampleDataPosition) { mPlayerBSamplePostions.push_back(sampleDataPosition); }

	//�ùķ��̼� �����Ŀ� �ʿ��� �����͵�
	const std::vector<tInnoSampleData>& GetPlayerASampleDatas() { return mPlayerASampleDatas; }
	const std::vector<float>&		GetPlayerBSamplePositions() { return mPlayerBSamplePostions; }

private:
	float mPlayerAPos;
	float mPlayerBPos;

	std::vector<tInnoSampleData> mPlayerASampleDatas;
	std::vector<float> mPlayerBSamplePostions;
};

