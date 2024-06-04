#pragma once
class InnoOJTClient
{
	PUBLIC_SINGLETON_DECLARE(InnoOJTClient);

public:
	void run();
	int Connect(const std::string& ip, const int port);

	void SendLog(const tPacketLog& packet);
	void SendPos(const tPacketPos& packet);
	void SendStop(const tPacketStop& packet);
	void SendStart(const tPacketStart& packet);
	void SendPosesSize(const tPacketPosesSize& packet);
	void SendPosesse(const tPacketPoses& packet);
};

