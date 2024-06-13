#pragma once
class InnoMessageQueue
{
public:
	PUBLIC_SINGLETON_DECLARE(InnoMessageQueue);

public:
	void PushPacket(const void* data, int dataSize);
	bool IsEmpty() { return mMessageQueue.empty(); }
	tPacketMessage GetNextMessage()
	{
		tPacketMessage message = mMessageQueue.front();
		mMessageQueue.pop();
		return message;
	}

private:
	std::vector<BYTE> mBytes;
	std::queue<tPacketMessage> mMessageQueue;
};

