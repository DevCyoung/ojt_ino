#pragma once
#include "CarSimulation.h"

class InnoOJTClient
{
	PUBLIC_SINGLETON_DECLARE(InnoOJTClient);

public:
	void run();
	int Connect(const std::string& ip, const int port);
	int DisConnect();

	void SendLog(int messageLen, const char* message);
	void SendPos(float pos);
	void SendStop();
	//void SendPosesSize(int size);
	void SendPoses(int size, const float* poses);

	void ReciveLog(const tPacketLog& outPacket);
	void RecivePos(const tPacketPos& outPacket);
	//void RecivePosesSize(const tPacketPosesSize& outPacket);
	void RecivePoses(const tPacketPoses& outPacket);
	void ReciveFinish(const tPacketFinish& outPacket);
	void ReciveStop(const tPacketStop& packet);
	void ReciveStart(const tPacketStart& packet);


	bool bServerTraining;
	bool bFinish;
	float direction;
	float poses[2];
	int mBSize;
	std::vector<float> mBPosArray;
	CarSimulation mSimulation;
};

