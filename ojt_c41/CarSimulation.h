#pragma once

struct tSampleData
{
	float SimTime;

	float ZsPos;
	float ZsSpeed;
	float ZsAcc;

	float ZuPos;
	float ZuSpeed;
	float ZuAcc;

	float Zr;

	float xPos;
	float xSpeed;
};

class CarSimulation
{	

public:
	CarSimulation();
	~CarSimulation();

	void SetSamplingTime(const float samplingTime) { mSamplingTime = samplingTime; }
	void SetMS(const float ms) { mMS = ms; }
	void SetKS(const float ks) { mKS = ks; }
	void SetCS(const float cs) { mCS = cs; }
	void SetKT(const float kt) { mKT = kt; }

	float GetSamplingTime() const { return mSamplingTime;  }
	float GetMS() const { return mMS; }
	float GetKS() const { return mKS; }
	float GetCS() const { return mCS; }
	float GetKT() const { return mKT; }

	void Update();

	void SaveData(const wchar_t* const filePath);

private:
	float mSamplingTime;
	float mMS;	//Sprung Mass
	float mMU;	//UnSprung Mass
	float mKS;	//코일 스프링
	float mCS;	//뎀프
	float mKT;	//
	float mSpeed;

	float mBumpStart;
	float mBumpEnd;
	float mBumpAmp;

	//
	float mSimTime;

	//
	//x[0] = Zs(위치)
	//x[1] = Zs(속도)
	//x[2] = Zu(위치)
	//x[3] = Zu(속도)	
	float x[4];

	//xdot[0] = Zs(속도)
	//xdot[1] = Zs(가속도)
	//xdot[2] = Zu(속도)
	//xdot[3] = Zu(가속도)
	float xdot[4];

	//
	std::vector<tSampleData> mSampleDatas;
};

