#include "pch.h"
#include "InnoOJTClient.h"
#include <iostream>
#include <LogListUI.h>
#include <PanelUIManager.h>
#include <TimeManager.h>
#include "InnoDataManager.h"

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
	, mCarDirection(1.f)
	, mCurPos{ 0, }
	, mBPosArray()
	, mServerSocket(INVALID_SOCKET)
	, mRecive()
	, mClientState(eClientState::None)
{
	InnoDataManager::initialize();
	InnoSimulator::initialize();
}

InnoOJTClient::~InnoOJTClient()
{
	CloseSocket(mServerSocket);

	if (mRecive.joinable())
	{
		mRecive.join();
	}	

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

	while (true)
	{
		int bytesReceived = recv(serverSocket, recvbuf, recvbuflen, 0);
		std::lock_guard<std::mutex> guard(gClientMutex);

		if (bytesReceived > 0)
		{
			ePacketID packetID = (ePacketID)getPacketId(recvbuf, recvbuflen);

			switch (packetID)
			{
			case Log:
			{
				tPacketLog log;
				deserializeData(recvbuf, sizeof(tPacketLog), &log);
				innoClient->ReciveLog(log);
			}
			break;
			case Pos:
			{
				tPacketPos pos;
				deserializeData(recvbuf, sizeof(tPacketPos), &pos);
				innoClient->RecivePos(pos);
			}
			break;
			case Start:
			{
				tPacketStart start;
				deserializeData(recvbuf, sizeof(tPacketStart), &start);
				innoClient->ReciveStart(start);
			}
			break;
			case Stop:
			{
				tPacketStop stop;
				deserializeData(recvbuf, sizeof(tPacketStop), &stop);
				innoClient->ReciveStop(stop);
			}			
			break;
			case Poses:
			{
				tPacketPoses poses;
				deserializeData(recvbuf, sizeof(tPacketPoses), &poses);
				innoClient->RecivePoses(poses);
			}
			break;
			case Finish:
			{
				tPacketFinish finish;
				deserializeData(recvbuf, sizeof(tPacketFinish), &finish);
				innoClient->ReciveFinish(finish);
			}
			break;
			default:
				break;
			}
		}
		else if (bytesReceived == 0)
		{			
			gLogListUIClient->WriteLine("Connection closed by server.");
			
			break;
		}
		else
		{
			gLogListUIClient->WriteError("Connection closed by server.");
			break;
		}
	}

	CloseSocket(serverSocket);
}

//서버에게 Connect 함수 (스레드)
static void ClientConnect(const std::string& ip, const int port, SOCKET* pServerSocket, std::thread* pRpecive, eClientState* clientState)
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
	if (INVALID_SOCKET  == *pServerSocket)
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

	//동역학 갱신
	if (mbServerTraining)
	{
		mCurPos[0] += gDeltaTime * 15.f * mCarDirection;
		SendPos(mCurPos[0]);
	}

	//mSimulator->Play();
	//mSimulator->Finish();
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

void InnoOJTClient::SendPoses(int size, const float* poses)
{
	send_poses(mServerSocket, size, poses);
}

void InnoOJTClient::ReciveLog(const tPacketLog& outPacket)
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient"));
	assert(logList);

	logList->WriteLine(outPacket.Message);
}

void InnoOJTClient::RecivePos(const tPacketPos& outPacket)
{
	mCurPos[1] = outPacket.Position;
}

void InnoOJTClient::RecivePoses(const tPacketPoses& outPacket)
{
	for (int i = 0; i < outPacket.Size; ++i)
	{
		mBPosArray.push_back(outPacket.Poses[i]);
	}
}

void InnoOJTClient::ReciveFinish(const tPacketFinish& outPacket)
{
	mbServerTrainingFinish = true;

	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient"));
	logList->WriteError("Training Finish");
}

void InnoOJTClient::ReciveStop(const tPacketStop& packet)
{
	mbServerTraining = false;
	//TODO
	//for (int i = 0; i < 100000; ++i)
	//{
	//	mSimulation.Update();
	//}
	//
	//int sampleCount = mSimulation.GetSampleDataCount();
	//std::queue<float> vecPoses;
	//
	//for (int i = 0; i < sampleCount; ++i)
	//{
	//	vecPoses.push(mSimulation.GetSampleData(i).xPos);
	//}

	//SendPosesSize(11);
	//TODO
	//while (!vecPoses.empty())
	//{
	//	std::vector<float> tempPoses;
	//	for (int j = 0; j < INNO_MAX_POS_SIZE; ++j)
	//	{
	//		if (vecPoses.empty())
	//		{
	//			break;
	//		}
	//
	//		tempPoses.push_back(vecPoses.front());
	//		vecPoses.pop();
	//	}
	//
	//	if (vecPoses.empty())
	//	{
	//		break;
	//	}
	//
	//	SendPoses(tempPoses.size(), tempPoses.data());
	//}
	//
	//SendStop();
}

void InnoOJTClient::ReciveStart(const tPacketStart& packet)
{
	mbServerTraining = true;
}
