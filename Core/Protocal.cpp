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
int getPacketId(char(&buffer)[], int recvSize) 
{
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

	int sendSize = send(socket, (const char*)&packet, sizeof(packet), 0);
	assert(sendSize != SOCKET_ERROR);
	assert(sendSize == sizeof(packet));
}

void send_pos(SOCKET socket, float pos)
{
	tPacketPos packet = {};

	packet.PacketID = ePacketID::Pos;
	packet.Position = pos;

	int sendSize = send(socket, (const char*)&packet, sizeof(packet), 0);
	assert(sendSize != SOCKET_ERROR);
	assert(sendSize == sizeof(packet));
}

void send_stop(SOCKET socket)
{
	tPacketStop packet = {};

	packet.PacketID = ePacketID::Stop;

	int sendSize = send(socket, (const char*)&packet, sizeof(packet), 0);
	assert(sendSize != SOCKET_ERROR);
	assert(sendSize == sizeof(packet));
}

void send_start(SOCKET socket)
{
	tPacketStart packet = {};

	packet.PacketID = ePacketID::Start;

	int sendSize = send(socket, (const char*)&packet, sizeof(packet), 0);
	assert(sendSize != SOCKET_ERROR);
	assert(sendSize == sizeof(packet));
}
