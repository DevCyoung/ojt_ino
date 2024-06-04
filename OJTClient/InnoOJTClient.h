#pragma once
class InnoOJTClient
{
	PUBLIC_SINGLETON_DECLARE(InnoOJTClient);

public:
	void run();
	int Connect(const std::string& ip, const int port);

	void SendLog(int messageLen, const char* message);
	void SendPos(float pos);
	void SendPosesSize(int size);
	void SendPoses(int size, const float* poses);

	void ReciveLog(const tPacketLog& outPacket);
	void RecivePos(const tPacketPos& outPacket);
	void RecivePosesSize(const tPacketPosesSize& outPacket);
	void RecivePoses(const tPacketPoses& outPacket);

	void ReciveStop(const tPacketStop& packet);
	void ReciveStart(const tPacketStart& packet);


	bool bServerTraining;

	float direction;
	float poses[2];
};

