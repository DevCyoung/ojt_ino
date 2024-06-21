#include "pch.h"
#include "Protocal.h"

void serializeData(const void* ori, size_t dataSize, void* packetBuffer)
{
	std::memcpy(packetBuffer, ori, dataSize);
}

// 역직렬화 함수
void deserializeData(const void* packetBuffer, size_t dataSize, void* ori)
{
	std::memcpy(ori, packetBuffer, dataSize);
}

// 패킷 ID를 확인하는 함수
int getPacketId(char(&buffer)[], int recvSize) 
{
	// 버퍼가 헤더 크기보다 작으면 유효한 패킷이 아님
	if (recvSize < sizeof(int))
	{
		return -1; // 0은 유효하지 않은 ID로 가정합니다.
	}

	// 패킷 헤더에서 패킷 ID를 읽음
	//PacketHeader header;
	int header = 0;
	std::memcpy(&header, buffer, sizeof(int));
	return header;
}

int getPacketId(const char* buffer)
{
	int header = 0;
	std::memcpy(&header, buffer, sizeof(int));
	return header;
}

void send(SOCKET socket, const void* data, int dataSize)
{
	const BYTE* pByte = (const BYTE*)data;

	while (1)
	{
		int sendSize = send(socket, (const char*)pByte, dataSize, 0);

		if (sendSize == SOCKET_ERROR)
		{
			assert(false);
		}
		else if (sendSize > dataSize)
		{
			assert(false);
		}


		if (sendSize == dataSize)
		{
			break;
		}

		pByte += sendSize;
		dataSize -= sendSize;
	}
}

void send_log(SOCKET socket, int messageLen, const char* message)
{
	tPacketLog packet = {};

	packet.PacketID = ePacketID::Log;
	packet.MessageLen = messageLen;
	memcpy(packet.Message, message, messageLen);

	send(socket, &packet, sizeof(tPacketLog));
}

void send_pos(SOCKET socket, float pos)
{
	tPacketPos packet = {};

	packet.PacketID = ePacketID::Pos;
	packet.Position = pos;

	send(socket, &packet, sizeof(tPacketPos));
}

void send_stop(SOCKET socket)
{
	tPacketStop packet = {};

	packet.PacketID = ePacketID::Stop;

	send(socket, &packet, sizeof(tPacketStop));
}

void send_start(SOCKET socket)
{
	tPacketStart packet = {};

	packet.PacketID = ePacketID::Start;

	send(socket, &packet, sizeof(tPacketStart));
}

void send_name(SOCKET socket, int nameLen, const char* name)
{
	tPacketName packet = {};

	packet.PacketID = ePacketID::Name;
	packet.NameLen = nameLen;
	memcpy(packet.Name, name, nameLen);

	send(socket, &packet, sizeof(tPacketName));
}
