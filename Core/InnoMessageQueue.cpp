#include "pch.h"
#include "InnoMessageQueue.h"

InnoMessageQueue::InnoMessageQueue()
	: mBytes()
{

}

InnoMessageQueue::~InnoMessageQueue()
{
}

void InnoMessageQueue::PushPacket(const void* data, int dataSize)
{
	for (int i = 0; i < dataSize; ++i)
	{
		mBytes.push_back(((const BYTE*)data)[i]);
	}

	while (mBytes.size() >= sizeof(int))
	{
		ePacketID packetID = (ePacketID)getPacketId((const char*)mBytes.data());

		tPacketMessage packetMessage = {};
		packetMessage.PacketID = packetID;

		if (Log == packetID)
		{
			constexpr int PACKET_SIZE = sizeof(tPacketLog);
			if (mBytes.size() < PACKET_SIZE)
			{
				break;
			}

			tPacketLog packet = {};
			deserializeData(mBytes.data(), PACKET_SIZE, &packet);
			mBytes.erase(mBytes.begin(), mBytes.begin() + PACKET_SIZE);

			packetMessage.MessageLen = packet.MessageLen;
			memcpy(packetMessage.buffer, packet.Message, INNO_MAX_POS_SIZE);

			mMessageQueue.push(packetMessage);
		}
		else if (Pos == packetID)
		{
			constexpr int PACKET_SIZE = sizeof(tPacketPos);
			if (mBytes.size() < PACKET_SIZE)
			{
				break;
			}

			tPacketPos packet = {};
			deserializeData(mBytes.data(), PACKET_SIZE, &packet);
			mBytes.erase(mBytes.begin(), mBytes.begin() + PACKET_SIZE);

			packetMessage.Position = packet.Position;

			mMessageQueue.push(packetMessage);
		}
		else if (Start == packetID)
		{
			constexpr int PACKET_SIZE = sizeof(tPacketStart);
			if (mBytes.size() < sizeof(tPacketStart))
			{
				break;
			}

			tPacketPos packet = {};
			deserializeData(mBytes.data(), PACKET_SIZE, &packet);
			mBytes.erase(mBytes.begin(), mBytes.begin() + PACKET_SIZE);

			mMessageQueue.push(packetMessage);
		}
		else if (Stop == packetID)
		{
			constexpr int PACKET_SIZE = sizeof(tPacketStop);
			if (mBytes.size() < sizeof(tPacketStop))
			{
				break;
			}

			tPacketPos packet = {};
			deserializeData(mBytes.data(), PACKET_SIZE, &packet);
			mBytes.erase(mBytes.begin(), mBytes.begin() + PACKET_SIZE);

			mMessageQueue.push(packetMessage);
		}
		else
		{
			assert(false);
		}

	}
}
