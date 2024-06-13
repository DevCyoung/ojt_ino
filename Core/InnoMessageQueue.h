#pragma once
#include "InnoOJTCore.h"

class InnoMessageQueue
{
public:
	PUBLIC_SINGLETON_DECLARE(InnoMessageQueue);

public:
	void PushRecivePacket(const void* data, int dataSize);
	void PushSendPacket(SOCKET socket, const void* data, int dataSize);
	void Clear();
	bool IsEmpty() { return mReciveMessageQueue.empty(); }
	tPacketMessage GetNextMessage()
	{
		tPacketMessage message = mReciveMessageQueue.front();
		mReciveMessageQueue.pop();
		return message;
	}

private:
	std::vector<BYTE> mReciveBytes;
	std::queue<tPacketMessage> mReciveMessageQueue;
};

