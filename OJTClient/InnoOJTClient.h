#pragma once

#include "InnoSimulator.h"

enum eClientState
{
	None,
	Connecting,
	Connected,
};

class InnoOJTClient
{
	PUBLIC_SINGLETON_DECLARE(InnoOJTClient);

public:
	void run();
	void DisConnect();

	int Connect(const std::string& ip, const int port);	
	bool IsConnecting() { return mClientState == eClientState::Connecting; }
	bool IsConnected() { return mClientState == eClientState::Connected; }

	std::string GetServerIP();
	int GetServerPort();

	void SendLog(int messageLen, const char* message);
	void SendPos(float pos);
	void SendStop();

	void ReciveLog(const tPacketLog& outPacket);
	void RecivePos(const tPacketPos& outPacket);	
	void ReciveStop(const tPacketStop& packet);
	void ReciveStart(const tPacketStart& packet);

	bool mbServerTraining;
	bool mbServerTrainingFinish;
	//bool mbConnect;
	//float mCarDirection;
	//float mCurPos[2];	
	//std::vector<float> mBPosArray;
	//InnoCarSimulation mSimulation;
	//InnoSimulator* mSimulator;
	SOCKET mServerSocket;
	std::thread mRecive;
	eClientState mClientState;
};

