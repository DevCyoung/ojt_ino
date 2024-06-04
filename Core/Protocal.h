#pragma once
class Protocal
{
};

enum ePacketID
{
	Log,
	Pos,
	Start,
	Stop,
	PosesSize,
	Poses,
};

struct tPacketLog
{
	ePacketID PacketID;
	int MessageLen;
	char Message[256];
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

struct tPacketPosesSize
{
	ePacketID PacketID;
	int Size;
};

struct tPacketPoses
{
	ePacketID PacketID;
	int Size;
	float Poses[256];
};

// 직렬화 함수
//ori 를 패킷버퍼로 바꿈
void serializeData(const void* ori, size_t dataSize, void* packetBuffer);

// 역직렬화 함수
void deserializeData(const void* packetBuffer, size_t dataSize, void* ori);

// 패킷 ID를 확인하는 함수
int getPacketId(char(&buffer)[], int recvSize);

typedef UINT_PTR SOCKET;
void send_log(SOCKET socket, int messageLen, const char* message);
void send_pos(SOCKET socket, float pos);
void send_poses_size(SOCKET socket, int size);
void send_poses(SOCKET socket, int size, const float* poses);
void send_stop(SOCKET socket);
void send_start(SOCKET socket);


//void send_pos()
//void send_