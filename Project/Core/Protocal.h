#pragma once
#define INNO_MAX_POS_SIZE 123
#define INNO_MAX_PACKET_SIZE 1024
#define INNO_AMX_PACKET_BUFFER_SIZE 1024
#define INNO_DEFAULT_PORT 5400

enum ePacketID
{
	Log,
	Pos,
	Start,
	Stop,
};

struct tPacketLog
{
	ePacketID PacketID;
	int MessageLen;
	char Message[INNO_MAX_POS_SIZE];
};

struct tPacketPos
{
	ePacketID PacketID;
	float Position;
};

struct tPacketStart
{
	ePacketID PacketID;
};

struct tPacketStop
{
	ePacketID PacketID;
};

struct tPacketMessage
{
	ePacketID PacketID;
	int MessageLen;
	float Position;
	char buffer[INNO_MAX_POS_SIZE];
};

// ����ȭ �Լ�
//ori �� ��Ŷ���۷� �ٲ�
void serializeData(const void* ori, size_t dataSize, void* packetBuffer);

// ������ȭ �Լ�
void deserializeData(const void* packetBuffer, size_t dataSize, void* ori);

//��Ŷ���ۿ��� ��ŶID�� Ȯ���ϴ� �Լ�
int getPacketId(char(&buffer)[], int recvSize);
int getPacketId(const char* buffer);

void send(SOCKET socket, const void* data, int dataSize);
void send_log(SOCKET socket, int messageLen, const char* message);
void send_pos(SOCKET socket, float pos);
void send_stop(SOCKET socket);
void send_start(SOCKET socket);