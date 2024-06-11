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
	void SetSpeed(const float Speed) { mSpeed = Speed; }

	float GetSamplingTime() const { return mSamplingTime;  }
	float GetMS() const { return mMS; }
	float GetKS() const { return mKS; }
	float GetCS() const { return mCS; }
	float GetKT() const { return mKT; }
	float GetSpeed() const { return mSpeed; }

	tSampleData CreateSampleData(float sampleTime);

	void Update();

	void SaveData(const wchar_t* const filePath);

	//const tSampleData& GetSampleData(const int idx) const { return mSampleDatas[idx]; }
	//size_t GetSampleDataCount() { return mSampleDatas.size(); }

private:
	float mSamplingTime;
	float mMS;	//Sprung Mass
	float mMU;	//UnSprung Mass
	float mKS;	//���� ������
	float mCS;	//����
	float mKT;	//
	float mSpeed;

	float mBumpStart;
	float mBumpEnd;
	float mBumpAmp;

	//
	float time;

	//
	//x[0] = Zs(��ġ)
	//x[1] = Zs(�ӵ�)
	//x[2] = Zu(��ġ)
	//x[3] = Zu(�ӵ�)	
	//float x[4];

	//xdot[0] = Zs(�ӵ�)
	//xdot[1] = Zs(���ӵ�)
	//xdot[2] = Zu(�ӵ�)
	//xdot[3] = Zu(���ӵ�)
	//float xdot[4];
};

