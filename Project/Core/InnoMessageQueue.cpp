#include "pch.h"
#include "InnoMessageQueue.h"

InnoMessageQueue::InnoMessageQueue()
	: mReciveBytes()
{

}

InnoMessageQueue::~InnoMessageQueue()
{
}

void InnoMessageQueue::PushRecivePacket(const void* data, int dataSize)
{
	for (int i = 0; i < dataSize; ++i)
	{
		mReciveBytes.push_back(((const BYTE*)data)[i]);
	}

	while (mReciveBytes.size() >= sizeof(int))
	{
		ePacketID packetID = (ePacketID)getPacketId((const char*)mReciveBytes.data());

		tPacketMessage packetMessage = {};
		packetMessage.PacketID = packetID;

		if (Log == packetID)
		{
			constexpr int PACKET_SIZE = sizeof(tPacketLog);
			if (mReciveBytes.size() < PACKET_SIZE)
			{
				break;
			}

			tPacketLog packet = {};
			deserializeData(mReciveBytes.data(), PACKET_SIZE, &packet);
			mReciveBytes.erase(mReciveBytes.begin(), mReciveBytes.begin() + PACKET_SIZE);

			packetMessage.MessageLen = packet.MessageLen;
			memcpy(packetMessage.buffer, packet.Message, INNO_MAX_POS_SIZE);

			mReciveMessageQueue.push(packetMessage);
		}
		else if (Pos == packetID)
		{
			constexpr int PACKET_SIZE = sizeof(tPacketPos);
			if (mReciveBytes.size() < PACKET_SIZE)
			{
				break;
			}

			tPacketPos packet = {};
			deserializeData(mReciveBytes.data(), PACKET_SIZE, &packet);
			mReciveBytes.erase(mReciveBytes.begin(), mReciveBytes.begin() + PACKET_SIZE);

			packetMessage.Position = packet.Position;

			mReciveMessageQueue.push(packetMessage);
		}
		else if (Start == packetID)
		{
			constexpr int PACKET_SIZE = sizeof(tPacketStart);
			if (mReciveBytes.size() < sizeof(tPacketStart))
			{
				break;
			}

			tPacketPos packet = {};
			deserializeData(mReciveBytes.data(), PACKET_SIZE, &packet);
			mReciveBytes.erase(mReciveBytes.begin(), mReciveBytes.begin() + PACKET_SIZE);

			mReciveMessageQueue.push(packetMessage);
		}
		else if (Stop == packetID)
		{
			constexpr int PACKET_SIZE = sizeof(tPacketStop);
			if (mReciveBytes.size() < sizeof(tPacketStop))
			{
				break;
			}

			tPacketPos packet = {};
			deserializeData(mReciveBytes.data(), PACKET_SIZE, &packet);
			mReciveBytes.erase(mReciveBytes.begin(), mReciveBytes.begin() + PACKET_SIZE);

			mReciveMessageQueue.push(packetMessage);
		}
		else
		{
			assert(false);
		}

	}
}

void InnoMessageQueue::PushSendPacket(SOCKET socket, const void* data, int dataSize)
{
	const BYTE* pByte = (const BYTE*)data;

	while (1)
	{
		int sendSize = send(socket, (const char*)&data, dataSize, 0);

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

		dataSize -= sendSize;
	}	
}

void InnoMessageQueue::Clear()
{
	mReciveBytes.clear();

	while (!mReciveMessageQueue.empty())
	{
		mReciveMessageQueue.pop();
	}
}