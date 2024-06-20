#include "pch.h"
#include "InnoOJTServer.h"
#include <TimeManager.h>
#include <InnoMessageQueue.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Iphlpapi.lib")

#define gLogListUI (static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI")))

static std::mutex gClientsMutex;
static SOCKET gListenSocket = INVALID_SOCKET;

std::string GetLocalIPAddress(SOCKET listenSocket)
{
	// Get the local IP address from the socket
	sockaddr_in addr;
	int addrLen = sizeof(addr);

	if (getsockname(listenSocket, (struct sockaddr*)&addr, &addrLen) == -1)
	{
		Assert(false, ASSERT_MSG_INVALID);
		return "NULL";
	}

	if (addr.sin_addr.s_addr == INADDR_ANY)
	{
		// If INADDR_ANY, we need to find the actual local address
		ULONG family = AF_INET;
		ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
		PIP_ADAPTER_ADDRESSES pAddresses = NULL;
		ULONG outBufLen = 0;
		DWORD dwRetVal = 0;

		dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);
		if (dwRetVal == ERROR_BUFFER_OVERFLOW)
		{
			pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
			if (pAddresses == NULL)
			{
				Assert(false, ASSERT_MSG_INVALID);
				return "";
			}
		}

		dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);
		if (dwRetVal == NO_ERROR)
		{
			for (PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses; pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next)
			{
				for (PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAddresses->FirstUnicastAddress; pUnicast != NULL; pUnicast = pUnicast->Next) {
					char ipAddressString[INET_ADDRSTRLEN] = { 0 };
					struct sockaddr_in* sa_in = (struct sockaddr_in*)pUnicast->Address.lpSockaddr;
					inet_ntop(AF_INET, &(sa_in->sin_addr), ipAddressString, INET_ADDRSTRLEN);

					// Skip loopback addresses
					if (strcmp(ipAddressString, "127.0.0.1") != 0)
					{
						free(pAddresses);
						return std::string(ipAddressString);
					}
				}
			}
		}
		else
		{
			Assert(false, ASSERT_MSG_INVALID);
		}

		if (pAddresses)
		{
			free(pAddresses);
		}
	}
	else
	{
		// Convert the address to a string
		char ipStr[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &(addr.sin_addr), ipStr, INET_ADDRSTRLEN) == NULL)
		{
			Assert(false, ASSERT_MSG_INVALID);
			return "NULL";
		}
		return std::string(ipStr);
	}

	return "NULL";
}



InnoOJTServer::InnoOJTServer()
	: mPanelManager(nullptr)
	, mChannelUI(nullptr)
	, mListenUI(nullptr)
	, mRoom{}
	, mClientThreads{}
	, mIP()
	, mServerState(eServerState::None)
{
	mPanelManager = PanelUIManager::GetInstance();
	mChannelUI = static_cast<ChannelUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("ChannelUI"));
	mListenUI = static_cast<ListenUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("ListenUI"));

	Assert(mPanelManager, ASSERT_MSG_NULL);
	Assert(mChannelUI, ASSERT_MSG_NULL);
	Assert(mListenUI, ASSERT_MSG_NULL);

	WSADATA wsaData;
	char hostName[256];
	struct addrinfo hints, * result, * ptr;
	char ip[INET_ADDRSTRLEN];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return;
	}

	if (gethostname(hostName, sizeof(hostName)) == SOCKET_ERROR) {		
		WSACleanup();
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;  // IPv4
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(hostName, nullptr, &hints, &result) != 0) {		
		WSACleanup();	
		return;
	}

	for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
		struct sockaddr_in* sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
		inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ip, sizeof(ip));
	}

	freeaddrinfo(result);
	WSACleanup();

	mIP = ip;

	InnoMessageQueue::initialize();

	return ;
}

InnoOJTServer::~InnoOJTServer()
{	
	DisConnect();
	InnoMessageQueue::deleteInstance();
}

// 클라이언트 핸들링 함수
static void handleClient(SOCKET clientSocket)
{
	char recvbuf[INNO_MAX_PACKET_SIZE];
	int recvbuflen = INNO_MAX_PACKET_SIZE;

	InnoOJTServer* innoServer = InnoOJTServer::GetInstance();

	while (true)
	{
		int bytesReceived = recv(clientSocket, recvbuf, recvbuflen, 0);
		std::lock_guard<std::mutex> guard(gClientsMutex);

		if (bytesReceived > 0)
		{
			InnoMessageQueue::GetInstance()->PushRecivePacket(recvbuf, bytesReceived);

			while (!InnoMessageQueue::GetInstance()->IsEmpty())
			{
				tPacketMessage pakcetMessage = InnoMessageQueue::GetInstance()->PopPacketMessage();

				ePacketID packetID = pakcetMessage.PacketID;
				int clientID = innoServer->GetInncoClient(clientSocket).ClientID;
				switch (packetID)
				{
				case Log:
				{
					tPacketLog packetLog = {};
					packetLog.PacketID = packetID;
					packetLog.MessageLen = pakcetMessage.MessageLen;
					memcpy(packetLog.Message, pakcetMessage.buffer, INNO_MAX_POS_SIZE);

					innoServer->ReciveLog(clientID, packetLog);
				}
					break;
				case Pos:
				{
					tPacketPos packetPos = {};
					packetPos.PacketID = packetID;
					packetPos.Position = pakcetMessage.Position;

					innoServer->RecivePos(clientID, packetPos);
				}
					break;
				default:
				{
					assert(false);
				}
					break;
				}

			}
		}
		else if (bytesReceived == 0)
		{			
			gLogListUI->WriteLine("Disconnect Client");
			innoServer->RemoveClient(clientSocket);
			break;
		}
		else
		{
			gLogListUI->WriteWarning("Disconnect Client");
			innoServer->RemoveClient(clientSocket);
			break;
		}
	}

	closesocket(clientSocket);
}

static void handleAccept()
{
	// 클라이언트 연결 및 처리
	while (INVALID_SOCKET != gListenSocket)
	{
		SOCKET clientSocket = accept(gListenSocket, NULL, NULL);

		if (INVALID_SOCKET == clientSocket)
		{
			gLogListUI->WriteLine("Accept handle close");
			closesocket(gListenSocket);
			gListenSocket = INVALID_SOCKET;
			continue;
		}

		{
			std::lock_guard<std::mutex> guard(gClientsMutex);
			InnoOJTServer::GetInstance()->Accept(clientSocket);
		}

	}
}

void InnoOJTServer::run()
{
	if (!IsListening())
	{
		return;
	}

	//Room Training
	static float trainingTime = 0.f;

	trainingTime += gDeltaTime;
	if (trainingTime < INN_FRAME_DELTA_TIME)
	{
		return;
	}

	while (trainingTime > INN_FRAME_DELTA_TIME)
	{
		trainingTime -= INN_FRAME_DELTA_TIME;
	}

	if (!mRoom.bTraining)
	{
		return;
	}

	//BroadCast
	for (int i = 0; i < mRoom.clients.size(); ++i)
	{
		for (int j = 0; j < mRoom.clients.size(); ++j)
		{
			if (i == j)
			{
				continue;
			}

			SendPos(mRoom.clients[i].ClientID, mRoom.curPoses[j]);
		}
	}
}

int InnoOJTServer::Listen(const int port)
{
	WSADATA wsaData;
	sockaddr_in serverAddr;

	//이미 Listen 중이라면
	if (gListenSocket != INVALID_SOCKET)
	{
		gLogListUI->WriteWarning("Already Listening.");
		return E_FAIL;
	}

	// Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		gLogListUI->WriteError("WSAStartup failed.");
		return E_FAIL;
	}

	// 소켓 생성
	gListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (gListenSocket == INVALID_SOCKET)
	{
		gLogListUI->WriteError("Socket creation failed.");
		WSACleanup();
		return E_FAIL;
	}

	// 서버 주소 및 포트 설정
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port);

	// 소켓을 서버 주소와 바인딩
	if (bind(gListenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		gLogListUI->WriteError("Bind failed.");
		closesocket(gListenSocket);
		WSACleanup();
		return E_FAIL;
	}

	// ListenSocket을 listen 상태로만듬
	if (listen(gListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		gLogListUI->WriteError("Listen failed");
		closesocket(gListenSocket);
		WSACleanup();
		return E_FAIL;
	}

	char buf[256] = { 0, };
	sprintf_s(buf, "Server is listening on port %d", port);
	gLogListUI->WriteLine(buf);

	// Listen 상태일때 ip 해당하는 소켓 ip로 변경
	mIP = GetLocalIPAddress(gListenSocket);
	Assert(mIP != "NULL", ASSERT_MSG_INVALID);

	std::thread accept(handleAccept);
	accept.detach();

	mServerState = eServerState::Listening;
	return S_OK;
}

int InnoOJTServer::Accept(SOCKET clientSocket)
{	
	char buff[256] = { 0, };
	sprintf_s(buff, "Accept: %s", GetClientIP(clientSocket).c_str());
	gLogListUI->WriteLine(buff);
	std::string ip(GetClientIP(clientSocket).c_str());

	tInnoClient innoClient = {};
	innoClient.ClientID = InnoOJTServer::serializeNumber++;
	innoClient.IP = ip;
	innoClient.Socket = clientSocket;

	mClients.push_back(innoClient);	
	mChannel.clients.push_back(innoClient);

	// 새로운 클라이언트를 처리하는 쓰레드 시작	
	mClientThreads[innoClient.ClientID] = std::thread(handleClient, clientSocket);
	return S_OK;
}

void InnoOJTServer::EnterRoom(int clientID)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);

	if (mRoom.clients.size() >= 2)
	{
		gLogListUI->WriteWarning("Room Clientes 2");
		return;
	}

	std::vector<tInnoClient>::iterator iter = mRoom.clients.begin();
	for (; iter != mRoom.clients.end(); ++iter)
	{
		if (iter->ClientID == clientID)
		{
			gLogListUI->WriteWarning("He's Already Room");
			return;
		}
	}

	tInnoClient client = {};
	if (TryGetInncoClient(clientID, &client))
	{
		gLogListUI->WriteLine("Enter room");
		mRoom.clients.push_back(client);
	}	
	else
	{
		gLogListUI->WriteWarning("Invalid client");
	}
}

void InnoOJTServer::ExitRoom(int clientID)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);

	std::vector<tInnoClient>::iterator iter = mRoom.clients.begin();
	for (; iter != mRoom.clients.end(); ++iter)
	{
		if (iter->ClientID == clientID)
		{
			mRoom.clients.erase(iter);
			gLogListUI->WriteLine("Exit Room");
			return;
		}
	}

	gLogListUI->WriteLine("Not exist user");
}

void InnoOJTServer::SendLog(int clientID, int messageLen, const char* message)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);
	tInnoClient client = GetInncoClient(clientID);

	send_log(client.Socket, messageLen, message);
}

void InnoOJTServer::SendPos(int clientID, float pos)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);
	tInnoClient client = GetInncoClient(clientID);

	send_pos(client.Socket, pos);
}

void InnoOJTServer::SendStop(int clientID)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);
	tInnoClient client = GetInncoClient(clientID);

	send_stop(client.Socket);
}

void InnoOJTServer::SendStart(int clientID)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);
	tInnoClient client = GetInncoClient(clientID);

	send_start(client.Socket);
}

void InnoOJTServer::ReciveLog(int clientID, const tPacketLog& outPacket)
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI"));
	logList->WriteLine(outPacket.Message);
}

void InnoOJTServer::RecivePos(int clientID, const tPacketPos& outPacket)
{
	for (int i = 0; i < mRoom.clients.size(); ++i)
	{
		if (clientID == mRoom.clients[i].ClientID)
		{
			mRoom.curPoses[i] = outPacket.Position;
			break;
		}
	}
}


std::string InnoOJTServer::GetClientIP(SOCKET clientSocket)
{
	sockaddr_in clientAddr;
	int addrLen = sizeof(clientAddr);

	// 클라이언트 소켓 주소 정보 가져오기
	if (getpeername(clientSocket, (sockaddr*)&clientAddr, &addrLen) == SOCKET_ERROR)
	{		
		return "NULL";
	}

	char ipStr[INET_ADDRSTRLEN];
	if (inet_ntop(AF_INET, &(clientAddr.sin_addr), ipStr, INET_ADDRSTRLEN) == NULL)
	{		
		return "v";
	}

	return std::string(ipStr);
}

static void removeClient(std::vector<tInnoClient>& clients, SOCKET socket)
{
	std::vector<tInnoClient>::iterator iter;

	iter = clients.begin();
	for (; iter != clients.end(); ++iter)
	{
		if (iter->Socket == socket)
		{
			clients.erase(iter);
			break;
		}
	}
}

void InnoOJTServer::RemoveClient(SOCKET socket)
{
	if (INVALID_SOCKET == socket)
	{
		return;
	}

	removeClient(mRoom.clients, socket);
	removeClient(mChannel.clients, socket);
	removeClient(mClients, socket);	
}

tInnoClient InnoOJTServer::GetInncoClient(SOCKET clientSocket)
{
	tInnoClient innoClient = {};

	if (false == TryGetInncoClient(clientSocket, &innoClient))
	{
		Assert(false, ASSERT_MSG_INVALID);
	}

	return innoClient;
}

tInnoClient InnoOJTServer::GetInncoClient(int clientID)
{
	tInnoClient innoClient = {};

	if (false == TryGetInncoClient(clientID, &innoClient))
	{
		Assert(false, ASSERT_MSG_INVALID);
	}

	return innoClient;
}

bool InnoOJTServer::TryGetInncoClient(SOCKET clientSocekt, tInnoClient* outInnoClient)
{
	std::vector<tInnoClient>::iterator iter = mClients.begin();

	bool bflag = false;

	for (; iter != mClients.end(); ++iter)
	{
		if (iter->Socket == clientSocekt)
		{
			bflag = true;
			*outInnoClient = *iter;
			break;
		}
	}

	return bflag;
}

bool InnoOJTServer::TryGetInncoClient(int clientID, tInnoClient* outInnoClient)
{
	std::vector<tInnoClient>::iterator iter = mClients.begin();

	bool bflag = false;

	for (; iter != mClients.end(); ++iter)
	{
		if (iter->ClientID == clientID)
		{
			bflag = true;
			*outInnoClient = *iter;
			break;
		}
	}

	return bflag;
}

void InnoOJTServer::DisConnect()
{
	std::vector<SOCKET> clientSockets;

	gClientsMutex.lock();

	for (int i = 0; i < mClients.size(); i++)
	{
		clientSockets.push_back(mClients[i].Socket);
	}

	//mRoom.clients.clear();
	RoomInit();

	mChannel.clients.clear();
	mClients.clear();

	gClientsMutex.unlock();

	for (int i = 0; i < clientSockets.size(); ++i)
	{
		closesocket(clientSockets[i]);
		clientSockets[i] = INVALID_SOCKET;
	}

	for (int i = 0; i < INNO_MAX_THREAD_SIZE; ++i)
	{
		if (mClientThreads[i].joinable())
		{
			mClientThreads[i].join();
		}
	}

	InnoMessageQueue::GetInstance()->Clear();

	closesocket(gListenSocket);
	gListenSocket = INVALID_SOCKET;
	mServerState = eServerState::None;
	serializeNumber = 0;
}
void InnoOJTServer::RoomInit()
{
	mRoom.bTraining = false;
	mRoom.clients.clear();
	for (int i = 0; i < INNO_MAX_ROOM_USER; ++i)
	{
		mRoom.curPoses[i] = 0.f;
	}
	mRoom.curTime = 0.f;
}