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
	Finish,
};

#define INNO_MAX_POS_SIZE 256
#define INNO_MAX_PACKET_SIZE 2048
#define INNO_DEFAULT_PORT 54000

typedef UINT_PTR SOCKET;

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
	float Poses[INNO_MAX_POS_SIZE];
};

struct tPacketFinish
{
	ePacketID PacketID;
};

// ����ȭ �Լ�
//ori �� ��Ŷ���۷� �ٲ�
void serializeData(const void* ori, size_t dataSize, void* packetBuffer);

// ������ȭ �Լ�
void deserializeData(const void* packetBuffer, size_t dataSize, void* ori);

//��Ŷ���ۿ��� ��ŶID�� Ȯ���ϴ� �Լ�
int getPacketId(char(&buffer)[], int recvSize);


void send_log(SOCKET socket, int messageLen, const char* message);
void send_pos(SOCKET socket, float pos);
void send_poses_size(SOCKET socket, int size);
void send_poses(SOCKET socket, int size, const float* poses);
void send_stop(SOCKET socket);
void send_start(SOCKET socket);
void send_finish(SOCKET socket);
