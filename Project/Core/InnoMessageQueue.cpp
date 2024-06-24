#include "pch.h"
#include "InnoMessageQueue.h"

InnoMessageQueue::InnoMessageQueue()
	: mReciveBytes()
	, mReciveMessageQueue()
{
	mReciveBytes.reserve(1000000);		
}

InnoMessageQueue::~InnoMessageQueue()
{
}

void InnoMessageQueue::PushRecivePacket(const void* data, int dataSize)
{
	const BYTE* pByte = reinterpret_cast<const BYTE*>(data);

	for (int i = 0; i < dataSize; ++i)
	{
		mReciveBytes.push_back(pByte[i]);
	}

	while (mReciveBytes.size() >= sizeof(int))
	{
		const ePacketID PACKET_ID = (ePacketID)getPacketId((const char*)mReciveBytes.data());

		tPacketMessage packetMessage = {};
		packetMessage.PacketID = PACKET_ID;

		if (Log == PACKET_ID)
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
			memcpy(packetMessage.buffer, packet.Message, INNO_MAX_BUFFER_SIZE);

			mReciveMessageQueue.push(packetMessage);
		}
		else if (Pos == PACKET_ID)
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
			packetMessage.Speed = packet.Speed;

			mReciveMessageQueue.push(packetMessage);
		}
		else if (Start == PACKET_ID)
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
		else if (Stop == PACKET_ID)
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
		else if (Name == PACKET_ID)
		{
			constexpr int PACKET_SIZE = sizeof(tPacketName);
			if (mReciveBytes.size() < sizeof(tPacketName))
			{
				break;
			}

			tPacketName packet = {};
			deserializeData(mReciveBytes.data(), PACKET_SIZE, &packet);
			mReciveBytes.erase(mReciveBytes.begin(), mReciveBytes.begin() + PACKET_SIZE);

			packetMessage.MessageLen = packet.NameLen;
			memcpy(packetMessage.buffer, packet.Name, INNO_MAX_BUFFER_SIZE);

			mReciveMessageQueue.push(packetMessage);
		}
		else
		{
			Assert(false, ASSERT_MSG_INVALID);
		}

	}
}

void InnoMessageQueue::PushSendPacket(const SOCKET socket, const void* data, int dataSize)
{
	const BYTE* pByte = (const BYTE*)data;

	while (1)
	{
		const int SEND_SIZE = send(socket, (const char*)&data, dataSize, 0);
		
		Assert(SEND_SIZE != SOCKET_ERROR, ASSERT_MSG_INVALID);
		Assert(SEND_SIZE <= dataSize, ASSERT_MSG_INVALID);

		if (SEND_SIZE == dataSize)
		{
			break;
		}

		dataSize -= SEND_SIZE;
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
