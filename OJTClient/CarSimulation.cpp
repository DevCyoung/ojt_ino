#include "pch.h"
#include "CarSimulation.h"
#include "fstream"

CarSimulation::CarSimulation()
	: mSamplingTime(0.001f)
	, mMS(363.5f)
	, mMU(37.6f)
	, mKS(10300.0f)
	, mCS(1000.0f)
	, mKT(18000.0f)
	, mSpeed(10.0f)
	, mBumpStart(0.0f)
	, mBumpEnd(3.0f)
	, mBumpAmp(0.05f)
	, time(0.0f)	
{
}

CarSimulation::~CarSimulation()
{
}

//1tick
tSampleData CarSimulation::CreateSampleData(float sampleTime)
{
	//1tick
	//mSimTime += mSamplingTime;
	float t1 = mBumpStart / mSpeed;
	float t2 = mBumpEnd / mSpeed;

	//도로의 높이
	float zr = 0.f;

	if (t1 <= sampleTime && sampleTime < t2)
	{
		zr = -mBumpAmp * sin(2.0f * XM_PI / (t2 - t1) / 2.0f * (sampleTime - t1));
	}

	float x[4] = { 0, };
	float xdot[4] = {0,};

	xdot[0] = x[1];
	xdot[1] = (-mKS * (x[0] - x[2]) - mCS * (x[1] - x[3])) / mMS;
	xdot[2] = x[3];
	xdot[3] = (mKS * (x[0] - x[2]) + mCS * (x[1] - x[3]) - mKT * (x[2] - zr)) / mMU;

	//적분
	x[0] = x[0] + mSamplingTime * xdot[0];
	x[1] = x[1] + mSamplingTime * xdot[1];
	x[2] = x[2] + mSamplingTime * xdot[2];
	x[3] = x[3] + mSamplingTime * xdot[3];

	static float testPos = 0.f;

	testPos += 5.f;

	tSampleData sampleData = {};

	sampleData.SimTime = sampleTime;
	sampleData.ZsPos = x[0];
	sampleData.ZsSpeed = x[1];
	sampleData.ZsAcc = xdot[1];
	sampleData.ZuPos = x[2];
	sampleData.ZuSpeed = x[3];
	sampleData.ZuAcc = xdot[3];
	sampleData.Zr = zr;
	sampleData.xPos += testPos;
	sampleData.xSpeed = mSpeed;

	return sampleData;
}

void CarSimulation::Update()
{	
	
}

void CarSimulation::SaveData(const wchar_t* const filePath)
{	
	std::wofstream outFile(filePath);

	wchar_t buffer[256] = L"Time\tZs Pos\tZs Speed\tZs Acc\tZu Pos\tZu Speed\tZu Acc\tZr\txPos\txSpeed";
	outFile << buffer;
	outFile << std::endl;
	//TODO
	//for (size_t i = 0; i < mSampleDatas.size(); ++i)
	//{
	//	outFile << mSampleDatas[i].SimTime;
	//	outFile << "\t";
	//	outFile << mSampleDatas[i].ZsPos;
	//	outFile << "\t";
	//	outFile << mSampleDatas[i].ZsSpeed;
	//	outFile << "\t";
	//	outFile << mSampleDatas[i].ZsAcc;
	//	outFile << "\t";
	//	outFile << mSampleDatas[i].ZuPos;
	//	outFile << "\t";
	//	outFile << mSampleDatas[i].ZuSpeed;
	//	outFile << "\t";
	//	outFile << mSampleDatas[i].ZuAcc;
	//	outFile << "\t";
	//	outFile << mSampleDatas[i].Zr;
	//	outFile << "\t";
	//	outFile << mSampleDatas[i].xPos;
	//	outFile << "\t";
	//	outFile << mSampleDatas[i].xSpeed;
	//	outFile << std::endl;
	//}
	outFile.close();
}
