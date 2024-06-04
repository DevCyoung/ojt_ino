#include "pch.h"
#include "InnoOJTClient.h"
#include <iostream>
#include <LogListUI.h>
#include <PanelUIManager.h>
#include <TimeManager.h>
#define DEFAULT_PORT 54000
#define DEFAULT_BUFLEN 2048

static SOCKET ConnectSocket = INVALID_SOCKET;
std::mutex client_mutex;
InnoOJTClient::InnoOJTClient()
	: bServerTraining(false)
	, poses{0, }
	, direction(1.f)
{
	
}

InnoOJTClient::~InnoOJTClient()
{

}

// 서버로부터 메시지 수신하는 함수
static void receive_messages(SOCKET ConnectSocket) {
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	InnoOJTClient* client = InnoOJTClient::GetInstance();

	while (true) 
	{

		//auto start = std::chrono::high_resolution_clock::now();  // 시작 시간 기록
		//int bytesReceived = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		//auto end = std::chrono::high_resolution_clock::now();  // 종료 시간 기록
		//if (bytesReceived > 0) {
		//	//std::cout << "Received: " << std::string(recvbuf, 0, bytesReceived) << "\n";
		//	std::chrono::duration<double> elapsed = end - start;
		//}
		//else if (bytesReceived == 0) {
		//	//std::cout << "Connection closed by server.\n";
		//	break;
		//}
		//else {
		//	//std::cerr << "recv failed.\n";
		//	break;
		//}

		int bytesReceived = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		ePacketID id = (ePacketID)getPacketId(recvbuf, recvbuflen);
		std::lock_guard<std::mutex> guard(client_mutex);

		if (bytesReceived > 0) 
		{
			switch (id)
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
				deserializeData(recvbuf, sizeof(tPacketPos), &start);
				client->ReciveStart(start);
			}
				break;
			case Stop:
			{
				tPacketStop stop;
				deserializeData(recvbuf, sizeof(tPacketPos), &stop);
				client->ReciveStop(stop);
			}
				break;
			case PosesSize:
			{
				tPacketPosesSize posesSize;
				deserializeData(recvbuf, sizeof(tPacketPosesSize), &posesSize);
				client->RecivePosesSize(posesSize);
			}
				break;
			case Poses:
			{
				tPacketPoses poses;
				deserializeData(recvbuf, sizeof(tPacketPoses), &poses);
				client->RecivePoses(poses);
			}
				break;
			default:
				break;
			}
			//	//std::cout << "Received: " << std::string(recvbuf, 0, bytesReceived) << "\n";
			//	std::chrono::duration<double> elapsed = end - start;
		}
		else if (bytesReceived == 0)
		{
			//std::cout << "Connection closed by server.\n";
		//	break;
		}
		else
		{

		}

	}

	closesocket(ConnectSocket);
	WSACleanup();
}


void InnoOJTClient::run()
{
	if (ConnectSocket == INVALID_SOCKET)
	{
		return;
	}

	if (bServerTraining)
	{
		poses[0] += gDeltaTime * 15.f * direction;
		SendPos(poses[0]);
	}

	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient"));


	static float time = 0.f;

	time += gDeltaTime;

	if (time >= 0.016f && bServerTraining)
	{		
		char buff[256] = { 0, };
		//sprintf_s(buff, "pos1:%f , pos2:%f", poses[0], poses[1]);
		//logList->WriteLine(buff);
		time = 0.f;
	}
}

int InnoOJTClient::Connect(const std::string& ip , const int port)
{
	WSADATA wsaData;
	
	sockaddr_in clientService;

	// Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		//std::cerr << "WSAStartup failed.\n";
		//return 1;
	}

	// 소켓 생성
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		//std::cerr << "Socket creation failed.\n";
		//WSACleanup();
		//return 1;
	}

	// 서버 주소 설정
	clientService.sin_family = AF_INET;
	clientService.sin_port = htons(DEFAULT_PORT);

	// 서버에 연결
	if (inet_pton(AF_INET, ip.c_str(), &clientService.sin_addr) <= 0) {
		//std::cerr << "Invalid address/ Address not supported \n";
		closesocket(ConnectSocket);
		WSACleanup();
		//return 1;
	}

	if (connect(ConnectSocket, (sockaddr*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		//std::cerr << "Connect failed.\n";
		closesocket(ConnectSocket);
		WSACleanup();
		//\return 1;
	}

	//std::cout << "Connected to server.\n";

	// 서버로부터 메시지를 받는 쓰레드 시작
	std::thread(receive_messages, ConnectSocket).detach();





	//Disconnect
	//closesocket(ConnectSocket);
	//WSACleanup();
	return 0;
}

void InnoOJTClient::SendLog(int messageLen, const char* message)
{
	send_log(ConnectSocket, messageLen, message);
}

void InnoOJTClient::SendPos(float pos)
{
	send_pos(ConnectSocket, pos);
}

void InnoOJTClient::SendPosesSize(int size)
{
	send_poses_size(ConnectSocket, size);
}

void InnoOJTClient::SendPoses(int size, const float* poses)
{
	send_poses(ConnectSocket, size, poses);
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

void InnoOJTClient::RecivePosesSize(const tPacketPosesSize& outPacket)
{
}

void InnoOJTClient::RecivePoses(const tPacketPoses& outPacket)
{
}

void InnoOJTClient::ReciveStop(const tPacketStop& packet)
{
}

void InnoOJTClient::ReciveStart(const tPacketStart& packet)
{	
	bServerTraining = true;
}
