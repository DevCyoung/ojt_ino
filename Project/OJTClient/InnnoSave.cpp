#include "pch.h"
#include "InnnoSave.h"
#include "InnoDataManager.h"
#include "InnoSimulator.h"
#include "EngineFile.h"

InnnoSave::InnnoSave()
	: mbZsPos(true)
	, mbZsSpeed(true)
	, mbZsAcc(true)
	, mbZuPos(true)
	, mbZuSpeed(true)
	, mbZuAcc(true)
	, mbZr(true)
	, mbXPos(true)
	, mbXSpeed(true)
	, mbXPosOther(true)
{
}

InnnoSave::~InnnoSave()
{

}

void InnnoSave::Save()
{
	const std::vector<float>& vecTimes		= InnoDataManager::GetInstance()->GetTimes();
	const std::vector<float>& vecZsPos		= InnoDataManager::GetInstance()->GetZsPoses();
	const std::vector<float>& vecZsSpeed	= InnoDataManager::GetInstance()->GetZsSpeeds();
	const std::vector<float>& vecZsAcc		= InnoDataManager::GetInstance()->GeetZsAccs();
	const std::vector<float>& vecZuPos		= InnoDataManager::GetInstance()->GetZuPoses();
	const std::vector<float>& vecZuSpeed	= InnoDataManager::GetInstance()->GetZuSpeeds();
	const std::vector<float>& vecZuAcc		= InnoDataManager::GetInstance()->GetZuAccs();
	const std::vector<float>& vecZr			= InnoDataManager::GetInstance()->GetZrs();
	const std::vector<float>& vecxPos		= InnoDataManager::GetInstance()->GetXPoses();	
	const std::vector<float>& vecXSpeed		= InnoDataManager::GetInstance()->GetXSpeeds();
	const std::vector<float>& vecXOtherPos	= InnoDataManager::GetInstance()->GetXOtherPoses();
	const float samplingTime				= InnoSimulator::GetInstance()->GetSamplingTime();

	float curSamplingTime = 0.f;

	int idx = INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND - 1;	
	Assert(idx <= vecTimes.size() + 1, ASSERT_MSG_INVALID);

	helper::DialogPath path = helper::SaveDialog();
	std::ofstream file(path.path);

	//file << "Time\tZsPos\tZsSpeed\tZsAcc\tZuPos\tZuSpeed\tZuAcc\tZr\txPos\txSpeed\txPosOther\n";
	file << "Time\t";	
	if (mbZsPos)
	{
		file << ZS_POS << "\t";
	}
	if (mbZsSpeed)
	{
		file << ZS_SPEED << "\t";
	}
	if (mbZsAcc)
	{
		file << ZS_ACC << "\t";
	}
	if (mbZuPos)
	{
		file << ZU_POS << "\t";
	}
	if (mbZuSpeed)
	{
		file << ZU_SPEED << "\t";
	}
	if (mbZuAcc)
	{
		file << ZU_ACC << "\t";
	}
	if (mbZr)
	{
		file << ZR << "\t";
	}
	if (mbXPos)
	{
		file << X_POS << "\t";
	}
	if (mbXSpeed)
	{
		file << X_SPEED << "\t";
	}
	if  (mbXPosOther)
	{
		file << X_POS_OTHER << "\t";
	}

	file << "\n";

	int t1 = idx;
	int t2 = idx + 1;

	while (curSamplingTime < vecTimes.back())
	{	
		if (vecTimes.size() <= t2)
		{
			break;
		}	
		else if (curSamplingTime > vecTimes[t2]) //현재 샘플링보다 큰 프레임을찾는다.
		{
			t1 = t2;
			++t2;
			continue;
		}

		//t1과 t2를 Lerp
		//이것으로 샘플링을한다.
		float t1Time = vecTimes[t1];
		float t2Time = vecTimes[t2];

		float per = (curSamplingTime - vecTimes[t1]) / (vecTimes[t2] - vecTimes[t1]);

		float ZsPos = vecZsPos[t1] + ((vecZsPos[t2] - vecZsPos[t1]) * per);
		float ZsSpeed = vecZsSpeed[t1] + ((vecZsSpeed[t2] - vecZsSpeed[t1]) * per);
		float ZsAcc = vecZsAcc[t1] + ((vecZsAcc[t2] - vecZsAcc[t1]) * per);
		float ZuPos = vecZuPos[t1] + ((vecZuPos[t2] - vecZuPos[t1]) * per);
		float ZuSpeed = vecZuSpeed[t1] + ((vecZuSpeed[t2] - vecZuSpeed[t1]) * per);
		float ZuAcc = vecZuAcc[t1] + ((vecZuAcc[t2] - vecZuAcc[t1]) * per);
		float Zr = vecZr[t1] + ((vecZr[t2] - vecZr[t1]) * per);
		float xPos = vecxPos[t1] + ((vecxPos[t2] - vecxPos[t1]) * per);
		float XSpeed = vecXSpeed[t1] + ((vecXSpeed[t2] - vecXSpeed[t1]) * per);
		float XOtherPos = vecXOtherPos[t1] + ((vecXOtherPos[t2] - vecXOtherPos[t1]) * per);

		// 데이터 작성
		file << curSamplingTime << "\t";

		if (mbZsPos)
		{
			file << ZsPos << "\t";
		}
		if (mbZsSpeed)
		{
			file << ZsSpeed << "\t";
		}
		if (mbZsAcc)
		{
			file << ZsAcc << "\t";
		}
		if (mbZuPos)
		{
			file << ZuPos << "\t";
		}
		if (mbZuSpeed)
		{
			file << ZuSpeed << "\t";
		}
		if (mbZuAcc)
		{
			file << ZuAcc << "\t";
		}
		if (mbZr)
		{
			file << Zr << "\t";
		}
		if (mbXPos)
		{
			file << xPos << "\t";
		}
		if (mbXSpeed)
		{
			file << XSpeed << "\t";
		}
		if (mbXPosOther)
		{
			file << XOtherPos << "\t";
		}

		file << "\n";
	
		curSamplingTime += samplingTime;
	}

	file.close();
}
