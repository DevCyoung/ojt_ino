#include "pch.h"		
#include "InnoOJTClient.h"
#include "InnoDataManager.h"
#include <InnoMessageQueue.h>	//메세지큐 헤더추가
#include "InnoInputUI.h"

#define gLogListUIClient (static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient")))
#define gInputUI (static_cast<InnoInputUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("InnoInputUI")))

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
	InnoMessageQueue::initialize();
}

InnoOJTClient::~InnoOJTClient()
{
	DisConnect();

	InnoMessageQueue::deleteInstance();
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
			//bytesReceived 가 0이상일때 패킷받기
			InnoMessageQueue::GetInstance()->PushRecivePacket(recvbuf, bytesReceived);

			//완성된 메세지가 하나라도있다면
			while (!InnoMessageQueue::GetInstance()->IsEmpty())
			{
				//완성된 메세지가 하나라도있다면 메세지를 꺼낸다 꺼낼때 메세지는 pop 되므로 주의
				tPacketMessage pakcetMessage = InnoMessageQueue::GetInstance()->PopPacketMessage();

				const ePacketID packetID = pakcetMessage.PacketID;

				switch (packetID)
				{
				case Log:
				{
					tPacketLog packetLog = {};
					packetLog.PacketID = packetID;
					packetLog.MessageLen = pakcetMessage.MessageLen;
					memcpy(packetLog.Message, pakcetMessage.buffer, INNO_MAX_POS_SIZE);

					gLogListUIClient->WriteLine("Recive: Log");

					innoClient->ReciveLog(packetLog);
				}					
					break;
				case Pos:
				{
					tPacketPos packetPos = {};
					packetPos.PacketID = packetID;
					packetPos.Position = pakcetMessage.Position;
					packetPos.Speed = pakcetMessage.Speed;

					//static float logTime = 0.f;
					//
					//logTime += gDeltaTime;
					//if (logTime >= 0.1f)
					//{
					//	char logBUff[256] = { 0, };
					//
					//	sprintf_s(logBUff, "pos: %.2f, speed: %.2f", packetPos.Position, packetPos.Speed);
					//
					//	gLogListUIClient->WriteLine(logBUff);
					//	logTime = 0.f;
					//}

					//다음에 들어오는시간
					static LARGE_INTEGER start;					
					float timeDelay = TimeManager::GetInstance()->EndTime(&start);
					static std::vector<float> times;
					times.push_back(timeDelay);
					innoClient->RecivePos(packetPos);					
					TimeManager::GetInstance()->StartTime(&start);
				}
					break;
				case Start:
				{
					tPacketStart packetStart = {};
					packetStart.PacketID = packetID;

					innoClient->ReciveStart(packetStart);

					gLogListUIClient->WriteLine("Recive: Start");
				}
					break;
				case Stop:
				{
					tPacketStop packetStop = {};
					packetStop.PacketID = packetID;

					innoClient->ReciveStop(packetStop);

					gLogListUIClient->WriteLine("Recive: Stop");
				}
					break;
				case Name:
				{
					tPacketName packetName = {};
					packetName.PacketID = packetID;

					innoClient->ReciveName(packetName);

					gLogListUIClient->WriteLine("Recive: Name");
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
			InnoSimulator::GetInstance()->ServerStop();

			gLogListUIClient->WriteLine("Connection closed by server.");
			innoClient->mClientState = eClientState::None;			
			closesocket(innoClient->mServerSocket);
			innoClient->mServerSocket = INVALID_SOCKET;
			break;
		}
		else
		{
			InnoSimulator::GetInstance()->ServerStop();

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

	static float time = 0;
	time += gDeltaTime;

	if (time >= 1 / 60.f)
	{
		time -= 1 / 60.f;

		//서버 트레이닝중이라면 내위치를 전송	
		if (mbServerTraining)
		{
			SendPos(InnoDataManager::GetInstance()->GetXPoses().back(), InnoSimulator::GetInstance()->GetSpeed());
		}
	}
	else
	{

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

	InnoMessageQueue::GetInstance()->Clear();

	mClientState = eClientState::None;
	mServerSocket = INVALID_SOCKET;
}

void InnoOJTClient::SendLog(int messageLen, const char* message)
{
	send_log(mServerSocket, messageLen, message);
}

void InnoOJTClient::SendPos(float pos, float speed)
{
	send_pos(mServerSocket, pos, speed);
}

void InnoOJTClient::SendStop()
{
	send_stop(mServerSocket);
}

void InnoOJTClient::SendName(int nameLen, const char* name)
{
	send_name(mServerSocket, nameLen, name);
}

void InnoOJTClient::ReciveLog(const tPacketLog& outPacket)
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient"));
	assert(logList);

	logList->WriteLine(outPacket.Message);

	//Other Car Symbol
	

	if (strlen(outPacket.Message) < strlen("Other Car : "))
	{
		return;
	}	

	int i = 0;
	for (; i < strlen(outPacket.Message); ++i)
	{
		if (outPacket.Message[i] == "Other Car : "[i])
		{
			continue;
		}
	}
}

void InnoOJTClient::RecivePos(const tPacketPos& outPacket)
{
	//서버로부터 B위치를 수신
	InnoSimulator::GetInstance()->SetPlayerBPos(outPacket.Position);
	InnoSimulator::GetInstance()->SetPlayerBSpeed(outPacket.Speed);
}

void InnoOJTClient::ReciveStop(const tPacketStop& packet)
{
	InnoSimulator::GetInstance()->Stop();

	mbServerTraining = false;
}

void InnoOJTClient::ReciveStart(const tPacketStart& packet)
{
	InnoSimulator::GetInstance()->ServerStart();

	mbServerTraining = true;
}

void InnoOJTClient::ReciveName(const tPacketName& packet)
{
	SendName(strlen(gInputUI->mName), gInputUI->mName);
}
