#pragma once
enum class eInnoSimulatorState
{
	None,
	Start,
	Playing,
	Stop,
	Editing,
	Finish,	
};

struct tInnoSampleData
{
	float Time;

	float ZsPos;
	float ZsSpeed;
	float ZsAcc;

	float ZuPos;
	float ZuSpeed;
	float ZuAcc;

	float Zr;

	float xPos;
	float xSpeed;

	float xPosOther;
};

class InnoSimulator
{
	PUBLIC_SINGLETON_DECLARE(InnoSimulator);

private:
	void start();
	void playing();
	void finish();

public:
	void SetPlayerBPos(float playerBPos) { mPlayerBPos = playerBPos; }
	void SetPlayerBSpeed(float playerBSpeed) { mPlayerBSpeed = playerBSpeed; }
	float GetPlayerPosB() { return mPlayerBPos; }
	float GetPlayerSpeedB() { return mPlayerBSpeed; }

	void Play();	
	void Stop();	
	void ServerStop();	
	void ServerStart();	
	void Finish();  

	void Update();
	eInnoSimulatorState GetSimulatorState() { return mState; }
	float GetCurTime() { return mCurTime; }
	float GetSpeed() { return mSpeed; }
	void SetMS(const float ms) { mMS = ms; }
	void SetMU(const float mu) { mMU = mu; }
	void SetKS(const float ks) { mKS = ks; }
	void SetCS(const float cs) { mCS = cs; }
	void SetKT(const float kt) { mKT = kt; }
	void SetSpeed(const float speed) 
	{ 		
		mSpeed = speed; 

		//스피드 최소값은 1
		if (mSpeed < 1.f)
		{
			mSpeed = 1.f;
		}
		//스피드 최대값은 30
		else if (mSpeed > 30.f)
		{
			mSpeed = 30.f;
		}
	}
	void SetSamplingTime(const float samplingTime) { mSamplingTime = samplingTime; }
	void SetBumpStart(float bumpStart) { mBumpStart = bumpStart; }
	void SetBumpEnd(float bumpEnd) { mBumpEnd = bumpEnd; }
	void SetBumpAmp(float bmpAmp) { mBumpAmp = bmpAmp; }
	void SetSamplintTIme(float samplingTime) { mSamplingTime = samplingTime; }

	void SetStartPos(float startPos) { mStartPos = startPos; }

	float GetStartPos() { return mStartPos; }
	float GetMS() const { return mMS; }
	float GetMU() const { return mMU; }
	float GetKS() const { return mKS; }
	float GetCS() const { return mCS; }
	float GetKT() const { return mKT; }
	float GetSpeed() const { return mSpeed; }
	float GetBumpStart() const { return mBumpStart; }
	float GetBumpEnd() const { return mBumpEnd; }
	float GetBumpAmp() const { return mBumpAmp; }
	float GetSamplingTime() { return mSamplingTime; }

	tInnoSampleData CreateSampleData(float sampleTime, float deltaTime);

	const std::vector< Vector3>& GetBumps() { return mBumps; }
	void PushBump(Vector3 bump);
	void RemoveBump(int idx);
	void SetBump(int idx, Vector3 bump);

	bool IsPlaying() { return mState == eInnoSimulatorState::Playing; }
	bool	mAccMode;
	float timeHistory;
	float mSpeedMin;
	float mSpeedMax;
private:
	void sort();

private:
	eInnoSimulatorState mState;
	float mCurTime;
	float mPlayerBPos;
	float mPlayerBSpeed;
	float mMS;
	float mMU;
	float mKS;
	float mCS;
	float mKT;
	float mSpeed;
	float mBumpStart;
	float mBumpEnd;
	float mBumpAmp;
	float mSamplingTime;
	float mFrameDeltaTime;
	float mPrevAPos;
	float mPrevBPos;	
	float mStartPos;
	float mX[4] = { 0, };
	float mXDot[4] = { 0, };

	std::vector<Vector3> mBumpsCopy;
	std::vector<Vector3> mBumps;
};
