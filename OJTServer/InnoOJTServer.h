#pragma once
//#include "Protocal.h"

class InnoOJTServer
{
	PUBLIC_SINGLETON_DECLARE(InnoOJTServer);

public:
	void run();

	int Listen(const int port);
	int Accept(SOCKET ClientSocket);

	void SendLog(const tPacketLog& packet);
	void SendPos(const tPacketPos& packet);
	void SendStop(const tPacketStop& packet);
	void SendStart(const tPacketStart& packet);
	void SendPosesSize(const tPacketPosesSize& packet);
	void SendPosesse(const tPacketPoses& packet);

	void ReciveLog(const tPacketLog& outPacket);
	void RecivePos(const tPacketPos& outPacket);
	void RecivePosesSize(const tPacketPosesSize& outPacket);
	void RecivePoses(const tPacketPoses& outPacket);
};

