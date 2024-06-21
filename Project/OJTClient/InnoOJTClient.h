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
	void SendName(int nameLen, const char* name);

	void ReciveLog(const tPacketLog& outPacket);
	void RecivePos(const tPacketPos& outPacket);	
	void ReciveStop(const tPacketStop& packet);
	void ReciveStart(const tPacketStart& packet);
	void ReciveName(const tPacketName& packet);

	bool mbServerTraining;
	bool mbServerTrainingFinish;	
	SOCKET mServerSocket;
	std::thread mRecive;
	eClientState mClientState;
};

