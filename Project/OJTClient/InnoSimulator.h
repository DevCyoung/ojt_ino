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
	//void SetPlayerAPos(float playerAPos) { mPlayerAPos = playerAPos; }
	void SetPlayerBPos(float playerBPos) { mPlayerBPos = playerBPos; }
	//float GetPlayerPosA() { return mPlayerAPos; }
	float GetPlayerPosB() { return mPlayerBPos; }

	void Play();	// �ùķ��̼��� �����մϴ� (None -> Start -> Playing)
	void Stop();	// �ùķ��̼��� �����մϴ�. �����͸�尡 �˴ϴ�. (Playing -> Stop-> Editing)
	void ServerStop();
	void ServerStart();	// �ùķ��̼��� ������ �����Ų��.
	void Finish();  // �����͸�带 �����մϴ�. (Edting -> Finish -> None)

	void Update();
	eInnoSimulatorState GetSimulatorState() { return mState; }
	float GetCurTime() { return mCurTime; }
	float GetSpeed() { return mSpeed; }
	void SetMS(const float ms) { mMS = ms; }
	void SetMU(const float mu) { mMU = mu; }
	void SetKS(const float ks) { mKS = ks; }
	void SetCS(const float cs) { mCS = cs; }
	void SetKT(const float kt) { mKT = kt; }
	void SetSpeed(const float speed) { mSpeed = speed; }
	void SetSamplingTime(const float samplingTime) { mSamplingTime = samplingTime; }
	void SetBumpStart(float bumpStart) { mBumpStart = bumpStart; }
	void SetBumpEnd(float bumpEnd) { mBumpEnd = bumpEnd; }
	void SetBumpAmp(float bmpAmp) { mBumpAmp = bmpAmp; }
	void SetSamplintTIme(float samplingTime) { mSamplingTime = samplingTime; }

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

private:
	eInnoSimulatorState mState;
	float mCurTime;
	float mPlayerBPos;
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
	float mPrevPos;

	float mX[4] = { 0, };
	float mXDot[4] = { 0, };

	std::vector<Vector3> mBumpsCopy;
	std::vector<Vector3> mBumps;
};


//float mMS;	//Sprung Mass
//float mMU;	//UnSprung Mass
//float mKS;	//���� ������
//float mCS;	//����
//float mKT;	//
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

//struct tInnoSampleData
//{
//	float Time;
//
//	float ZsPos;
//	float ZsSpeed;
//	float ZsAcc;
//
//	float ZuPos;
//	float ZuSpeed;
//	float ZuAcc;
//
//	float Zr;
//
//	float xPos;
//	float xSpeed;
//};

//void InnoQuarterCarSimulator::SaveData(const wchar_t* const filePath)
//{	
//	std::wofstream outFile(filePath);
//
//	wchar_t buffer[256] = L"Time\tZs Pos\tZs Speed\tZs Acc\tZu Pos\tZu Speed\tZu Acc\tZr\txPos\txSpeed";
//	outFile << buffer;
//	outFile << std::endl;
//	//TODO
//	//for (size_t i = 0; i < mSampleDatas.size(); ++i)
//	//{
//	//	outFile << mSampleDatas[i].SimTime;
//	//	outFile << "\t";
//	//	outFile << mSampleDatas[i].ZsPos;
//	//	outFile << "\t";
//	//	outFile << mSampleDatas[i].ZsSpeed;
//	//	outFile << "\t";
//	//	outFile << mSampleDatas[i].ZsAcc;
//	//	outFile << "\t";
//	//	outFile << mSampleDatas[i].ZuPos;
//	//	outFile << "\t";
//	//	outFile << mSampleDatas[i].ZuSpeed;
//	//	outFile << "\t";
//	//	outFile << mSampleDatas[i].ZuAcc;
//	//	outFile << "\t";
//	//	outFile << mSampleDatas[i].Zr;
//	//	outFile << "\t";
//	//	outFile << mSampleDatas[i].xPos;
//	//	outFile << "\t";
//	//	outFile << mSampleDatas[i].xSpeed;
//	//	outFile << std::endl;
//	//}
//	outFile.close();
//}