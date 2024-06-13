#include "pch.h"
#include "InnoOJTClient.h"
#include <iostream>
#include <LogListUI.h>
#include <PanelUIManager.h>
#include <TimeManager.h>
#include "InnoDataManager.h"
#include "InnoClientMessageQueue.h"
#define gLogListUIClient (static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient")))

static std::mutex gClientMutex;

static int CloseSocket(const SOCKET serverSocket)
{
	if (INVALID_SOCKET == serverSocket)
	{
		return S_FALSE;
	}
	else if (0 != closesocket(serverSocket))
	{
		return S_FALSE;
	}

	WSACleanup();
	return S_OK;
}

static int getPort(SOCKET socket) {
	sockaddr_in addr;
	int addrLen = sizeof(addr);

	if (getpeername(socket, (sockaddr*)&addr, &addrLen) == SOCKET_ERROR) {
		return -1;
	}

	return ntohs(addr.sin_port);
}

static std::string getIPAddress(SOCKET socket) {
	sockaddr_in addr;
	int addrLen = sizeof(addr);

	if (getpeername(socket, (sockaddr*)&addr, &addrLen) == SOCKET_ERROR) {
		//std::cerr << "Failed to get IP address: " << WSAGetLastError() << std::endl;
		return "";
	}

	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));

	return std::string(ip);
}

InnoOJTClient::InnoOJTClient()
	: mbServerTraining(false)
	, mbServerTrainingFinish(false)
	, mServerSocket(INVALID_SOCKET)
	, mRecive()
	, mClientState(eClientState::None)
{
	InnoDataManager::initialize();
	InnoSimulator::initialize();
	InnoClientMessageQueue::initialize();
}

InnoOJTClient::~InnoOJTClient()
{
	DisConnect();

	InnoClientMessageQueue::initialize();
	InnoSimulator::deleteInstance();
	InnoDataManager::deleteInstance();
}

std::string InnoOJTClient::GetServerIP()
{
	return getIPAddress(mServerSocket);
}

int InnoOJTClient::GetServerPort()
{
	return getPort(mServerSocket);
}

// 서버로부터 메시지 수신하는 함수
static void ClientRecive(SOCKET serverSocket)
{
	char recvbuf[INNO_MAX_PACKET_SIZE];
	int recvbuflen = INNO_MAX_PACKET_SIZE;

	InnoOJTClient* const innoClient = InnoOJTClient::GetInstance();

	char buffer[INNO_AMX_PACKET_BUFFER_SIZE];

	while (true)
	{
		int bytesReceived = recv(serverSocket, recvbuf, recvbuflen, 0);

		std::lock_guard<std::mutex> guard(gClientMutex);


		if (bytesReceived > 0)
		{			
			ePacketID packetID = (ePacketID)getPacketId(recvbuf, recvbuflen);
			//Message						

			switch (packetID)
			{
			case Log:
			{
				assert(bytesReceived == sizeof(tPacketLog));

				tPacketLog log;
				deserializeData(recvbuf, sizeof(tPacketLog), &log);
				innoClient->ReciveLog(log);
			}
			break;
			case Pos:
			{
				assert(bytesReceived == sizeof(tPacketPos));

				tPacketPos pos;
				deserializeData(recvbuf, sizeof(tPacketPos), &pos);
				innoClient->RecivePos(pos);
			}
			break;
			case Start:
			{
				assert(bytesReceived == sizeof(tPacketStart));

				tPacketStart start;
				deserializeData(recvbuf, sizeof(tPacketStart), &start);
				innoClient->ReciveStart(start);
				gLogListUIClient->WriteLine("Recive: Start");
			}
			break;
			case Stop:
			{
				assert(bytesReceived == sizeof(tPacketStop));

				tPacketStop stop;
				deserializeData(recvbuf, sizeof(tPacketStop), &stop);
				innoClient->ReciveStop(stop);
				gLogListUIClient->WriteLine("Recive: Stop");
			}
			break;
			default:
			{
				//assert(bytesReceived == sizeof(tPacketLog));

				char errorBuff[256] = {};
				sprintf_s(errorBuff, "Invalid Packet %d", packetID);
				gLogListUIClient->WriteLine(errorBuff);
			}
			break;
			}
		}
		else if (bytesReceived == 0)
		{
			gLogListUIClient->WriteLine("Connection closed by server.");
			innoClient->mClientState = eClientState::None;
			closesocket(innoClient->mServerSocket);
			innoClient->mServerSocket = INVALID_SOCKET;
			break;
		}
		else
		{
			gLogListUIClient->WriteError("Connection closed by server.");
			innoClient->mClientState = eClientState::None;
			closesocket(innoClient->mServerSocket);
			innoClient->mServerSocket = INVALID_SOCKET;
			break;
		}
	}

	CloseSocket(serverSocket);
}

//서버에게 Connect 함수 (스레드)
void ClientConnect(const std::string& ip, const int port, SOCKET* pServerSocket, std::thread* pRpecive, eClientState* clientState)
{
	std::lock_guard<std::mutex> guard(gClientMutex);

	WSADATA wsaData;
	sockaddr_in clientService;

	// Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		gLogListUIClient->WriteError("WSAStartup failed.");
		*clientState = eClientState::None;
		return;
	}

	//클라이언트소켓 생성
	*pServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == *pServerSocket)
	{
		gLogListUIClient->WriteError("Socket creation failed.");
		*clientState = eClientState::None;
		return;
	}

	// 서버 주소 설정
	clientService.sin_family = AF_INET;
	clientService.sin_port = htons(port);

	//소켓에 바인딩한다.
	if (inet_pton(AF_INET, ip.c_str(), &clientService.sin_addr) <= 0)
	{
		gLogListUIClient->WriteError("Invalid address Address not supported .");
		CloseSocket(*pServerSocket);
		*pServerSocket = INVALID_SOCKET;
		*clientState = eClientState::None;
		return;
	}

	if (connect(*pServerSocket, (sockaddr*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		gLogListUIClient->WriteError("Connect failed");
		CloseSocket(*pServerSocket);
		*pServerSocket = INVALID_SOCKET;
		*clientState = eClientState::None;
		return;
	}

	//서버와 연결됨
	gLogListUIClient->WriteLine("Connect Success!");

	// 서버로부터 메시지를 받는 쓰레드 시작
	// 쓰레드 초기화
	if (pRpecive->joinable())
	{
		pRpecive->join();
	}

	*pRpecive = std::thread(ClientRecive, *pServerSocket);
	*clientState = eClientState::Connected;
	return;
}

void InnoOJTClient::run()
{
	InnoSimulator::GetInstance()->Update();

	if (mServerSocket == INVALID_SOCKET)
	{
		return;
	}

	//서버 트레이닝중이라면 내위치를 전송	
	if (mbServerTraining)
	{
		SendPos(InnoDataManager::GetInstance()->GetXPoses().back());
	}
}

int InnoOJTClient::Connect(const std::string& ip, const int port)
{
	if (eClientState::None == mClientState)
	{
		mClientState = eClientState::Connecting;
		std::thread(ClientConnect, ip, port, &mServerSocket, &mRecive, &mClientState).detach();
	}
	else if (eClientState::Connecting == mClientState)
	{
		return S_OK;
	}

	if (INVALID_SOCKET != mServerSocket)
	{
		return S_OK;
	}

	return S_OK;
}

void InnoOJTClient::DisConnect()
{
	CloseSocket(mServerSocket);

	if (mRecive.joinable())
	{
		mRecive.join();
	}

	mClientState = eClientState::None;
	mServerSocket = INVALID_SOCKET;
}

void InnoOJTClient::SendLog(int messageLen, const char* message)
{
	send_log(mServerSocket, messageLen, message);
}

void InnoOJTClient::SendPos(float pos)
{
	send_pos(mServerSocket, pos);
}

void InnoOJTClient::SendStop()
{
	send_stop(mServerSocket);
}

void InnoOJTClient::ReciveLog(const tPacketLog& outPacket)
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient"));
	assert(logList);

	logList->WriteLine(outPacket.Message);
}

void InnoOJTClient::RecivePos(const tPacketPos& outPacket)
{
	//서버로부터 B위치를 수신
	InnoSimulator::GetInstance()->SetPlayerBPos(outPacket.Position);
}

void InnoOJTClient::ReciveStop(const tPacketStop& packet)
{
	mbServerTraining = false;
	InnoSimulator::GetInstance()->Stop();
}

void InnoOJTClient::ReciveStart(const tPacketStart& packet)
{
	mbServerTraining = true;
	InnoSimulator::GetInstance()->Play();
}
