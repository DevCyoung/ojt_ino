#include "pch.h"
#include "Protocal.h"

void serializeData(const void* ori, size_t dataSize, void* packetBuffer)
{
	std::memcpy(packetBuffer, ori, dataSize);
}

// ������ȭ �Լ�
void deserializeData(const void* packetBuffer, size_t dataSize, void* ori)
{
	std::memcpy(ori, packetBuffer, dataSize);
}

// ��Ŷ ID�� Ȯ���ϴ� �Լ�
int getPacketId(char(&buffer)[], int recvSize) {
	// ���۰� ��� ũ�⺸�� ������ ��ȿ�� ��Ŷ�� �ƴ�
	if (recvSize < sizeof(int))
	{
		return -1; // 0�� ��ȿ���� ���� ID�� �����մϴ�.
	}

	// ��Ŷ ������� ��Ŷ ID�� ����
	//PacketHeader header;
	int header = 0;
	std::memcpy(&header, buffer, sizeof(int));
	return header;
}

void send_log(SOCKET socket, int messageLen, const char* message)
{
	tPacketLog packet = {};

	packet.PacketID = ePacketID::Log;
	packet.MessageLen = messageLen;
	memcpy(packet.Message, message, messageLen);

	if (send(socket, (const char*)&packet, sizeof(packet), 0) == SOCKET_ERROR)
	{
		assert(false);
	}
}

void send_pos(SOCKET socket, float pos)
{
	tPacketPos packet = {};

	packet.PacketID = ePacketID::Pos;
	packet.Position = pos;

	if (send(socket, (const char*)&packet, sizeof(packet), 0) == SOCKET_ERROR)
	{
		assert(false);
	}
}

void send_poses(SOCKET socket, int size, const float* poses)
{
	tPacketPoses packet = {};

	packet.Size = size;
	packet.PacketID = ePacketID::Poses;
	memcpy(packet.Poses, poses, sizeof(float) * size);

	if (send(socket, (const char*)&packet, sizeof(packet), 0) == SOCKET_ERROR)
	{
		assert(false);
	}
}

void send_stop(SOCKET socket)
{
	tPacketPoses packet = {};

	packet.PacketID = ePacketID::Stop;

	if (send(socket, (const char*)&packet, sizeof(packet), 0) == SOCKET_ERROR)
	{
		assert(false);
	}
}

void send_start(SOCKET socket)
{
	tPacketPoses packet = {};

	packet.PacketID = ePacketID::Start;

	if (send(socket, (const char*)&packet, sizeof(packet), 0) == SOCKET_ERROR)
	{
		assert(false);
	}
}

void send_finish(SOCKET socket)
{
	tPacketFinish packet = {};

	packet.PacketID = ePacketID::Finish;

	if (send(socket, (const char*)&packet, sizeof(packet), 0) == SOCKET_ERROR)
	{
		assert(false);
	}
}

