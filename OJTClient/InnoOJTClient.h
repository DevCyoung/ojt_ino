#pragma once
class InnoOJTClient
{
	PUBLIC_SINGLETON_DECLARE(InnoOJTClient);

public:
	void run();
	int Connect(const std::string& ip, const int port);

	void SendLog(const tPacketLog& packet);
	void SendPos(const tPacketPos& packet);
	
	void SendPosesSize(const tPacketPosesSize& packet);
	void SendPosesse(const tPacketPoses& packet);

	void ReciveLog(const tPacketLog& outPacket);
	void RecivePos(const tPacketPos& outPacket);
	void RecivePosesSize(const tPacketPosesSize& outPacket);
	void RecivePoses(const tPacketPoses& outPacket);

	void ReciveStop(const tPacketStop& packet);
	void ReciveStart(const tPacketStart& packet);
};

