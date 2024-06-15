#include "pch.h"
#include "InnnoSave.h"
#include "InnoDataManager.h"
#include "InnoSimulator.h"
#include "EngineFile.h"
#include <fstream>

InnnoSave::InnnoSave()
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
	file << "Time\tZsPos\tZsSpeed\tZsAcc\tZuPos\tZuSpeed\tZuAcc\tZr\txPos\txSpeed\txPosOther\n";

	int t1 = idx;
	int t2 = idx + 1;

	while (curSamplingTime < vecTimes.back())
	{
		//현재 샘플링보다 큰 프레임을찾는다.

		float time = vecTimes[t2];
		if (curSamplingTime > time)
		{
			t1 = t2;
			t2 = ++t2;
		}

		if (idx >= vecTimes.size())
		{
			break;
		}

		//t1과 t2를 Lerp
		//이것으로 샘플링을한다.
		float t1Time = vecTimes[t1];
		float t2Time = vecTimes[t2];
		while (curSamplingTime <= t2Time)
		{			
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
			float XOtherPos = vecXOtherPos[t1] + ((vecXOtherPos[t2] - vecXOtherPos[t1])* per);
			// 데이터 작성
			file << curSamplingTime << "\t"
				 << ZsPos			<< "\t"
				 << ZsSpeed			<< "\t"
				 << ZsAcc			<< "\t"
				 << ZuPos			<< "\t"
				 << ZuSpeed			<< "\t"
				 << ZuAcc			<< "\t"
				 << Zr				<< "\t"
				 << xPos			<< "\t"
				 << XSpeed			<< "\t"
				 << XOtherPos		<< "\n";

			curSamplingTime += samplingTime;
		}		
	}

	file.close();
}
