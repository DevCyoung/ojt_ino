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

// ����ȭ �Լ�
//ori �� ��Ŷ���۷� �ٲ�
void serializeData(const void* ori, size_t dataSize, void* packetBuffer);

// ������ȭ �Լ�
void deserializeData(const void* packetBuffer, size_t dataSize, void* ori);