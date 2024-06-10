#include "pch.h"
#include "InnoOJTClient.h"
#include <iostream>
#include <LogListUI.h>
#include <PanelUIManager.h>
#include <TimeManager.h>

#define gLogListUI (static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient")))

static SOCKET gConnectSocket = INVALID_SOCKET;
static std::mutex gClientMutex;

InnoOJTClient::InnoOJTClient()
	: bServerTraining(false)
	, bFinish(false)
	, direction(1.f)
	, poses{ 0, }
	, mBSize(0)
	, mBPosArray()
	, mSimulation()
{
}

InnoOJTClient::~InnoOJTClient()
{
}

// 서버로부터 메시지 수신하는 함수
static void ClientRecive(SOCKET connectSocket)
{
	char recvbuf[INNO_MAX_PACKET_SIZE];
	int recvbuflen = INNO_MAX_PACKET_SIZE;

	InnoOJTClient* client = InnoOJTClient::GetInstance();

	while (true)
	{
		int bytesReceived = recv(connectSocket, recvbuf, recvbuflen, 0);
		std::lock_guard<std::mutex> guard(gClientMutex);
		ePacketID packetID = (ePacketID)getPacketId(recvbuf, recvbuflen);

		if (bytesReceived > 0)
		{
			switch (packetID)
			{
			case Log:
			{
				tPacketLog log;
				deserializeData(recvbuf, sizeof(tPacketLog), &log);
				client->ReciveLog(log);
			}
			break;
			case Pos:
			{
				tPacketPos pos;
				deserializeData(recvbuf, sizeof(tPacketPos), &pos);
				client->RecivePos(pos);
			}
			break;
			case Start:
			{
				tPacketStart start;
				deserializeData(recvbuf, sizeof(tPacketStart), &start);
				client->ReciveStart(start);
			}
			break;
			case Stop:
			{
				tPacketStop stop;
				deserializeData(recvbuf, sizeof(tPacketStop), &stop);
				client->ReciveStop(stop);
			}
			break;
			case PosesSize:
			{
				//tPacketPosesSize posesSize;
				//deserializeData(recvbuf, sizeof(tPacketPosesSize), &posesSize);
				//client->RecivePosesSize(posesSize);
				gLogListUI->WriteError("PosesSize not invalied packet");
			}
			break;
			case Poses:
			{
				tPacketPoses poses;
				deserializeData(recvbuf, sizeof(tPacketPoses), &poses);
				client->RecivePoses(poses);
			}
			break;
			case Finish:
			{
				tPacketFinish finish;
				deserializeData(recvbuf, sizeof(tPacketFinish), &finish);
				client->ReciveFinish(finish);
			}
			break;
			default:
				break;
			}
		}
		else if (bytesReceived == 0)
		{			
			gLogListUI->WriteLine("Connection closed by server.");
			break;
		}
		else
		{
		}
	}

	//DisConnect
	closesocket(connectSocket);
	WSACleanup();
}


void InnoOJTClient::run()
{
	if (gConnectSocket == INVALID_SOCKET)
	{
		return;
	}

	//동역학 갱신
	if (bServerTraining)
	{
		poses[0] += gDeltaTime * 15.f * direction;
		SendPos(poses[0]);
	}
}

int InnoOJTClient::Connect(const std::string& ip, const int port)
{
	WSADATA wsaData;
	sockaddr_in clientService;

	// Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		gLogListUI->WriteError("WSAStartup failed.");
		return E_FAIL;
	}

	//클라이언트소켓 생성
	gConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (gConnectSocket == INVALID_SOCKET)
	{
		gLogListUI->WriteError("Socket creation failed.");
		return E_FAIL;
	}

	// 서버 주소 설정
	clientService.sin_family = AF_INET;
	clientService.sin_port = htons(INNO_DEFAULT_PORT);

	// 서버에 연결
	if (inet_pton(AF_INET, ip.c_str(), &clientService.sin_addr) <= 0)
	{
		gLogListUI->WriteError("Invalid address Address not supported .");
		closesocket(gConnectSocket);
		WSACleanup();
		return E_FAIL;
	}

	if (connect(gConnectSocket, (sockaddr*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		gLogListUI->WriteError("Connect failed");
		closesocket(gConnectSocket);
		WSACleanup();
		return E_FAIL;
	}

	//서버와 연결됨
	gLogListUI->WriteLine("Connected to server.");

	// 서버로부터 메시지를 받는 쓰레드 시작
	std::thread(ClientRecive, gConnectSocket).detach();
	return S_OK;
}

void InnoOJTClient::SendLog(int messageLen, const char* message)
{
	send_log(gConnectSocket, messageLen, message);
}

void InnoOJTClient::SendPos(float pos)
{
	send_pos(gConnectSocket, pos);
}

void InnoOJTClient::SendStop()
{
	send_stop(gConnectSocket);
}

//void InnoOJTClient::SendPosesSize(int size)
//{
//	send_poses_size(gConnectSocket, size);
//}

void InnoOJTClient::SendPoses(int size, const float* poses)
{
	send_poses(gConnectSocket, size, poses);
}

void InnoOJTClient::ReciveLog(const tPacketLog& outPacket)
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient"));
	assert(logList);

	logList->WriteLine(outPacket.Message);
}

void InnoOJTClient::RecivePos(const tPacketPos& outPacket)
{
	poses[1] = outPacket.Position;
}

//void InnoOJTClient::RecivePosesSize(const tPacketPosesSize& outPacket)
//{	
//	mBSize = outPacket.Size;
//}			

void InnoOJTClient::RecivePoses(const tPacketPoses& outPacket)
{
	for (int i = 0; i < outPacket.Size; ++i)
	{
		mBPosArray.push_back(outPacket.Poses[i]);
	}
}

void InnoOJTClient::ReciveFinish(const tPacketFinish& outPacket)
{
	bFinish = true;

	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient"));
	logList->WriteError("Training Finish");
}

void InnoOJTClient::ReciveStop(const tPacketStop& packet)
{
	bServerTraining = false;

	for (int i = 0; i < 100000; ++i)
	{
		mSimulation.Update();
	}

	int sampleCount = mSimulation.GetSampleDataCount();
	std::queue<float> vecPoses;

	for (int i = 0; i < sampleCount; ++i)
	{
		vecPoses.push(mSimulation.GetSampleData(i).xPos);
	}

	//SendPosesSize(11);

	while (!vecPoses.empty())
	{
		std::vector<float> tempPoses;
		for (int j = 0; j < INNO_MAX_POS_SIZE; ++j)
		{
			if (vecPoses.empty())
			{
				break;
			}

			tempPoses.push_back(vecPoses.front());
			vecPoses.pop();
		}

		if (vecPoses.empty())
		{
			break;
		}

		SendPoses(tempPoses.size(), tempPoses.data());
	}

	SendStop();
}

void InnoOJTClient::ReciveStart(const tPacketStart& packet)
{
	bServerTraining = true;
}
