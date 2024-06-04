#pragma once
//#include "Protocal.h"

#include "PanelUIManager.h"
#include "ChannelUI.h"
#include "ListenUI.h"
#include "LogListUI.h"
#include "PanelUIManager.h"

struct tInnoClient
{
	int id;
	SOCKET socket;
};

class InnoOJTServer
{
	PUBLIC_SINGLETON_DECLARE(InnoOJTServer);

public:
	void run();

	int Listen(const int port);
	int Accept(SOCKET ClientSocket);

	void SendLog(const tPacketLog& packet);
	void SendPos(const tPacketPos& packet);
	void SendStop(const tPacketStop& packet);
	void SendStart(const tPacketStart& packet);
	void SendPosesSize(const tPacketPosesSize& packet);
	void SendPosesse(const tPacketPoses& packet);

	void ReciveLog(const tPacketLog& outPacket);
	void RecivePos(const tPacketPos& outPacket);
	void RecivePosesSize(const tPacketPosesSize& outPacket);
	void RecivePoses(const tPacketPoses& outPacket);

	static inline int serializeNumber = 0;

	void AddClient(SOCKET socket) { mClients.push_back({ InnoOJTServer::serializeNumber++, socket });}
	void Disconnect(SOCKET socket)
	{ 
		std::vector<tInnoClient>::iterator iter = mClients.begin();
		for (; iter != mClients.end(); ++iter)
		{
			if (iter->socket == socket)
			{
				mClients.erase(iter);
				break;
			}

		}		
		closesocket(socket);
	}

private:
	std::vector<tInnoClient> mClients;

public:
	PanelUIManager* mPanelManager;
	ChannelUI* mChannel;
	LogListUI* mLogListUI;
	ListenUI* mListenUI;
};

