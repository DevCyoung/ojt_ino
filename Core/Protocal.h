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