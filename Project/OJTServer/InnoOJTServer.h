#pragma once
#include "PanelUIManager.h"
#include "ChannelUI.h"
#include "ListenUI.h"
#include <LogListUI.h>
#include <PanelUIManager.h>

#define INNO_MAX_ROOM_USER 8
#define INNO_MAX_THREAD_SIZE 2048

enum eServerState
{
	None,	
	Listening
};

struct tInnoClient
{
	int ClientID;
	std::string IP;
	SOCKET Socket;
};

struct tInnoChanel
{
	std::vector<tInnoClient> clients;
};

struct tInnoRoom
{
	bool bTraining;
	float curPoses[INNO_MAX_ROOM_USER];
	float curTime;
	std::vector<tInnoClient> clients;
};

class InnoOJTServer
{
	PUBLIC_SINGLETON_DECLARE(InnoOJTServer);

public:
	void run();

	int Listen(const int port);
	bool IsListening() { return mServerState == eServerState::Listening; }

	int Accept(SOCKET ClientSocket);

	void EnterRoom(int clientID);
	void ExitRoom(int clientID);

	void SendLog(int clientID, int messageLen, const char* message);
	void SendPos(int clientID, float pos);
	void SendStop(int clientID);
	void SendStart(int clientID);	

	void ReciveLog(int clientID, const tPacketLog& outPacket);
	void RecivePos(int clientID, const tPacketPos& outPacket);
	//void ReciveStop(int clientID, const tPacketStop& outPacket);	

	void RemoveClient(const SOCKET clientSocket);

	void DisConnect();

	void RoomInit();

	static inline int serializeNumber = 0;
	tInnoClient GetInncoClient(SOCKET socket);
	tInnoClient GetInncoClient(int clientID);
	bool TryGetInncoClient(SOCKET socket, tInnoClient* outInnoClient);
	bool TryGetInncoClient(int clientID, tInnoClient* outInnoClient);

	std::string GetClientIP(SOCKET clientSocket);
	std::string GetServerIP() { return mIP; }

public:
	PanelUIManager* mPanelManager;
	ChannelUI* mChannelUI;	
	ListenUI* mListenUI;

	tInnoChanel mChannel;
	tInnoRoom mRoom;

	std::vector<tInnoClient> mClients;
	std::thread mClientThreads[INNO_MAX_THREAD_SIZE];

	std::string mIP;	

	eServerState mServerState;
};
