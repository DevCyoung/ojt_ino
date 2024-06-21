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
	Name,
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

struct tPacketName
{
	ePacketID PacketID;
	int NameLen;
	char Name[INNO_MAX_POS_SIZE];
};

struct tPacketMessage
{
	ePacketID PacketID;
	int MessageLen;
	float Position;
	char buffer[INNO_MAX_POS_SIZE];
};

// 직렬화 함수
void serializeData(const void* ori, size_t dataSize, void* packetBuffer);

// 역직렬화 함수
void deserializeData(const void* packetBuffer, size_t dataSize, void* ori);

//패킷버퍼에서 패킷ID를 확인하는 함수
int getPacketId(char(&buffer)[], int recvSize);
int getPacketId(const char* buffer);

void send(SOCKET socket, const void* data, int dataSize);
void send_log(SOCKET socket, int messageLen, const char* message);
void send_pos(SOCKET socket, float pos);
void send_stop(SOCKET socket);
void send_start(SOCKET socket);
void send_name(SOCKET socket, int nameLen, const char* name);
