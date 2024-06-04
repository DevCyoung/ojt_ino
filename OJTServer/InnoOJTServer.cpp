#include "pch.h"
#include "InnoOJTServer.h"
#include "LogListUI.h"
#include "PanelUIManager.h"
static std::vector<SOCKET> clients;
static std::mutex clients_mutex;
static SOCKET ListenSocket = INVALID_SOCKET;

#define DEFAULT_PORT 54000



std::string GetClientIP(SOCKET clientSocket) {
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


//// 패킷 헤더 구조체
//struct PacketHeader {
//	int packetId;
//};

// 패킷 ID를 확인하는 함수
int getPacketId(char (&buffer)[512], int recvSize) {
	// 버퍼가 헤더 크기보다 작으면 유효한 패킷이 아님
	if (recvSize < sizeof(int))
	{
		return -1; // 0은 유효하지 않은 ID로 가정합니다.
	}

	// 패킷 헤더에서 패킷 ID를 읽음
	//PacketHeader header;
	int header = 0;
	std::memcpy(&header, buffer, sizeof(int));
	return header;
}



// 클라이언트 핸들링 함수
static void handle_client(SOCKET client_socket) {
	char recvbuf[512];
	int recvbuflen = 512;
	int result;

	InnoOJTServer* server = InnoOJTServer::GetInstance();
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI"));

	while ((result = recv(client_socket, recvbuf, recvbuflen, 0)) > 0)
	{
		//auto start = std::chrono::high_resolution_clock::now();  // 시작 시간 기록
		//recvbuf[result] = '\0';
		//std::string message = std::string(recvbuf, result);
		////std::cout << "Received: " << message << "\n";
		//
		//// 브로드캐스트 메시지
		//std::lock_guard<std::mutex> guard(clients_mutex);
		//for (SOCKET client : clients)
		//{
		//	if (client != client_socket) 
		//	{
		//		send(client, message.c_str(), message.size(), 0);
		//	}
		//}

		int packetId = getPacketId(recvbuf, recvbuflen);

		std::lock_guard<std::mutex> guard(clients_mutex);
		switch (packetId)
		{
		case Log:
		{
			tPacketLog log;
			deserializeData(recvbuf, sizeof(tPacketLog), &log);
			server->ReciveLog(log);
		}
			break;
		case Pos:
		{
			tPacketPos pos;
			deserializeData(recvbuf, sizeof(tPacketPos), &pos);
			server->RecivePos(pos);
		}
			break;		
		case PosesSize:
		{
			tPacketPosesSize posesSize;
			deserializeData(recvbuf, sizeof(tPacketPosesSize), &posesSize);
			server->RecivePosesSize(posesSize);
		}
			break;
		case Poses:
		{
			tPacketPoses poses;
			deserializeData(recvbuf, sizeof(tPacketPoses), &poses);
			server->RecivePoses(poses);
		}
			break;
		default:			
			logList->WriteLine("invalie packet");
			break;
		}

		//auto end = std::chrono::high_resolution_clock::now();  // 종료 시간 기록
		//std::chrono::duration<double> elapsed = end - start;
		//std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";  // 경과 시간 출력
	}

	// 클라이언트 소켓을 리스트에서 제거
	std::lock_guard<std::mutex> guard(clients_mutex);
	clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
	closesocket(client_socket);
}

static void handle_accept()
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI"));

	// 클라이언트 연결 및 처리
	while (ListenSocket != INVALID_SOCKET)
	{
		SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);

		if (ClientSocket == INVALID_SOCKET)
		{			
			logList->WriteLine("Accept failed.");
			closesocket(ListenSocket);
			WSACleanup();			
		}

		{
			std::lock_guard<std::mutex> guard(clients_mutex);
			InnoOJTServer::GetInstance()->Accept(ClientSocket);
			char buff[256] = { 0, };
			sprintf_s(buff, "%s %s", GetClientIP(ClientSocket).c_str(), "Enter");
			logList->WriteLine(buff);
			clients.push_back(ClientSocket);
		}

		// 새로운 클라이언트를 처리하는 쓰레드 시작
		std::thread(handle_client, ClientSocket).detach();		
	}
}


InnoOJTServer::InnoOJTServer()
{
}

InnoOJTServer::~InnoOJTServer()
{

}

void InnoOJTServer::run()
{

}

int InnoOJTServer::Listen(const int port)
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI"));
	assert(logList);

	WSADATA wsaData;
	sockaddr_in serverAddr;

	// Winsock 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	{
		logList->WriteLine("WSAStartup failed.");
		return 1;
	}

	// 소켓 생성
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) 
	{
		logList->WriteLine("Socket creation failed.");
		WSACleanup();
		return 1;
	}

	// 서버 주소 및 포트 설정
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(DEFAULT_PORT);

	// 소켓을 서버 주소와 바인딩
	if (bind(ListenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
	{
		logList->WriteLine("Bind failed.");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// 연결 대기
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) 
	{
		logList->WriteLine("Listen failed");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	char buf[256] = { 0, };
	sprintf_s(buf, "Server is listening on port %d", port);
	logList->WriteLine(buf);

	std::thread accept(handle_accept);
	accept.detach();

	return 0;
}

int InnoOJTServer::Accept(SOCKET ClientSocket)
{
	//GetClientIP(ClientSocket);
	return 0;
}

void InnoOJTServer::SendLog(const tPacketLog& packet)
{
}

void InnoOJTServer::SendPos(const tPacketPos& packet)
{
}

void InnoOJTServer::SendStop(const tPacketStop& packet)
{
}

void InnoOJTServer::SendStart(const tPacketStart& packet)
{
}

void InnoOJTServer::SendPosesSize(const tPacketPosesSize& packet)
{
}

void InnoOJTServer::SendPosesse(const tPacketPoses& packet)
{
}























void InnoOJTServer::ReciveLog(const tPacketLog& outPacket)
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI"));

	logList->WriteLine(outPacket.Message);
}

void InnoOJTServer::RecivePos(const tPacketPos& outPacket)
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI"));
	char buff[512] = { 0, };
	sprintf_s(buff, "%f", outPacket.Position);
	logList->WriteLine(buff);
}

void InnoOJTServer::RecivePosesSize(const tPacketPosesSize& outPacket)
{
}

void InnoOJTServer::RecivePoses(const tPacketPoses& outPacket)
{
}
