#pragma once
#include "PanelUIManager.h"
#include "ChannelUI.h"
#include "ListenUI.h"
#include "LogListUI.h"
#include "PanelUIManager.h"

#define INNO_MAX_ROOM_USER 8

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

	float poses[INNO_MAX_ROOM_USER];
	int	posSize[INNO_MAX_ROOM_USER];
	bool bStop[INNO_MAX_ROOM_USER];

	std::vector<float> posesArray[INNO_MAX_ROOM_USER];
	std::vector<tInnoClient> clients;
};

class InnoOJTServer
{
	PUBLIC_SINGLETON_DECLARE(InnoOJTServer);

public:
	void run();

	int Listen(const int port);
	int Accept(SOCKET ClientSocket);

	void EnterRoom(int clientID);
	void ExitRoom(int clientID);

	void SendLog(int clientID, int messageLen, const char* message);
	void SendPos(int clientID, float pos);
	void SendStop(int clientID);
	void SendStart(int clientID);	
	void SendPoses(int clientID, int size, const float* poses);

	void ReciveLog(int clientID, const tPacketLog& outPacket);
	void RecivePos(int clientID, const tPacketPos& outPacket);
	void ReciveStop(int clientID, const tPacketStop& outPacket);	
	void RecivePoses(int clientID, const tPacketPoses& outPacket);
	void AddClient(SOCKET socket);
	void Disconnect(SOCKET socket);	
	static inline int serializeNumber = 0;
	tInnoClient GetInncoClient(SOCKET socket);
	tInnoClient GetInncoClient(int clientID);

	std::string GetClientIP(SOCKET clientSocket);
private:
	

public:
	PanelUIManager* mPanelManager;
	ChannelUI* mChannelUI;	
	ListenUI* mListenUI;

	tInnoChanel mChannel;
	tInnoRoom mRoom;

	std::vector<tInnoClient> mClients;
};

