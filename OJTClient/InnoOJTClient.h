#pragma once

#include "InnoSimulator.h"
class InnoOJTClient
{
	PUBLIC_SINGLETON_DECLARE(InnoOJTClient);

public:
	void run();
	int Connect(const std::string& ip, const int port);	

	void SendLog(int messageLen, const char* message);
	void SendPos(float pos);
	void SendStop();	
	void SendPoses(int size, const float* poses);

	void ReciveLog(const tPacketLog& outPacket);
	void RecivePos(const tPacketPos& outPacket);
	void RecivePoses(const tPacketPoses& outPacket);
	void ReciveFinish(const tPacketFinish& outPacket);
	void ReciveStop(const tPacketStop& packet);
	void ReciveStart(const tPacketStart& packet);

	bool mbServerTraining;
	bool mbServerTrainingFinish;
	float mCarDirection;
	float mCurPos[2];	
	std::vector<float> mBPosArray;
	//InnoCarSimulation mSimulation;
	//InnoSimulator* mSimulator;
	SOCKET mServerSocket;
	std::thread mRecive;
};

