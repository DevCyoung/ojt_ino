#pragma once
#include "PanelUIManager.h"
#include "ChannelUI.h"
#include "ListenUI.h"
#include "LogListUI.h"
#include "PanelUIManager.h"

struct tInnoClient
{
	int id;
	std::string ip;
	SOCKET socket;
};

struct tInnoChanel
{
	std::vector<tInnoClient> clients;
};

struct tInnoRoom
{
	bool bTraining;
	std::vector<tInnoClient> clients;
};

class InnoOJTServer
{
	PUBLIC_SINGLETON_DECLARE(InnoOJTServer);

public:
	void run();

	int Listen(const int port);
	int Accept(SOCKET ClientSocket);

	void EnterRoom(int id);
	void ExitRoom(int id);

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

	std::string GetClientIP(SOCKET clientSocket)
	{
		sockaddr_in clientAddr;
		int addrLen = sizeof(clientAddr);

		// 클라이언트 소켓 주소 정보 가져오기
		if (getpeername(clientSocket, (sockaddr*)&clientAddr, &addrLen) == SOCKET_ERROR)
		{
			//std::cerr << "getpeername failed: " << WSAGetLastError() << std::endl;
			return "";
		}

		char ipStr[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &(clientAddr.sin_addr), ipStr, INET_ADDRSTRLEN) == NULL)
		{
			//std::cerr << "inet_ntop failed: " << WSAGetLastError() << std::endl;
			return "";
		}

		return std::string(ipStr);
	}

	void AddClient(SOCKET socket) 
	{ 
		char buff[256] = { 0, };
		sprintf_s(buff, "%s %s", GetClientIP(socket).c_str(), "Enter");
		mLogListUI->WriteLine(buff);

		std::string ip(buff);
		mClients.push_back({ InnoOJTServer::serializeNumber++, ip, socket });

	}
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


	tInnoClient GetInncoClient(SOCKET socket)
	{
		std::vector<tInnoClient>::iterator iter = mClients.begin();
		for (; iter != mClients.end(); ++iter)
		{
			if (iter->socket == socket)
			{
				return *iter;
			}
		}

		assert(false);
	}

	tInnoClient GetInncoClient(int id)
	{
		std::vector<tInnoClient>::iterator iter = mClients.begin();
		for (; iter != mClients.end(); ++iter)
		{
			if (iter->id == id)
			{
				return *iter;
			}

		}

		assert(false);
	}

private:
	

public:
	PanelUIManager* mPanelManager;
	ChannelUI* mChannelUI;
	LogListUI* mLogListUI;
	ListenUI* mListenUI;

	tInnoChanel mChannel;
	tInnoRoom mRoom;

	std::vector<tInnoClient> mClients;	
};

