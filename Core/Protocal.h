#pragma once
#define INNO_MAX_POS_SIZE 256
#define INNO_MAX_PACKET_SIZE 2048
#define INNO_DEFAULT_PORT 5400

typedef UINT_PTR SOCKET;

enum ePacketID
{
	Log,
	Pos,
	Start,
	Stop,	
	Poses,
	Finish,
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

struct tPacketPoses
{
	ePacketID PacketID;
	int Size;
	float Poses[INNO_MAX_POS_SIZE];
};

struct tPacketFinish
{
	ePacketID PacketID;
};

// 직렬화 함수
//ori 를 패킷버퍼로 바꿈
void serializeData(const void* ori, size_t dataSize, void* packetBuffer);

// 역직렬화 함수
void deserializeData(const void* packetBuffer, size_t dataSize, void* ori);

//패킷버퍼에서 패킷ID를 확인하는 함수
int getPacketId(char(&buffer)[], int recvSize);


void send_log(SOCKET socket, int messageLen, const char* message);
void send_pos(SOCKET socket, float pos);
void send_poses(SOCKET socket, int size, const float* poses);
void send_stop(SOCKET socket);
void send_start(SOCKET socket);
void send_finish(SOCKET socket);
