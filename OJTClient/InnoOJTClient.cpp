#include "pch.h"
#include "InnoOJTClient.h"
#include <iostream>

#define DEFAULT_PORT 54000
#define DEFAULT_BUFLEN 512

static SOCKET ConnectSocket = INVALID_SOCKET;

InnoOJTClient::InnoOJTClient()
{

}

InnoOJTClient::~InnoOJTClient()
{

}

// 서버로부터 메시지 수신하는 함수
static void receive_messages(SOCKET ConnectSocket) {
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	while (true) {
		auto start = std::chrono::high_resolution_clock::now();  // 시작 시간 기록
		int bytesReceived = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		auto end = std::chrono::high_resolution_clock::now();  // 종료 시간 기록
		if (bytesReceived > 0) {
			//std::cout << "Received: " << std::string(recvbuf, 0, bytesReceived) << "\n";
			std::chrono::duration<double> elapsed = end - start;
		}
		else if (bytesReceived == 0) {
			//std::cout << "Connection closed by server.\n";
			break;
		}
		else {
			//std::cerr << "recv failed.\n";
			break;
		}
	}

	closesocket(ConnectSocket);
	WSACleanup();
}


void InnoOJTClient::run()
{
	std::string sendbuf;
	//while (true) 
	//{
	//	std::getline(std::cin, sendbuf);
	//
	//	auto start = std::chrono::high_resolution_clock::now();  // 시작 시간 기록
	//	if (send(ConnectSocket, sendbuf.c_str(), sendbuf.length(), 0) == SOCKET_ERROR) {
	//		std::cerr << "Send failed.\n";
	//		break;
	//	}
	//	auto end = std::chrono::high_resolution_clock::now();  // 종료 시간 기록
	//	std::chrono::duration<double> elapsed = end - start;
	//}
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

void InnoOJTClient::SendLog(const tPacketLog& packet)
{
	char buff[sizeof(tPacketLog)];

	serializeData(&packet, sizeof(tPacketLog), buff);
	if (send(ConnectSocket, buff, sizeof(tPacketLog), 0) == SOCKET_ERROR)
	{
		assert(false);
	}
}

void InnoOJTClient::SendPos(const tPacketPos& packet)
{
	char buff[sizeof(tPacketPos)];

	serializeData(&packet, sizeof(tPacketPos), buff);
	if (send(ConnectSocket, buff, sizeof(tPacketPos), 0) == SOCKET_ERROR)
	{
		assert(false);
	}
}

void InnoOJTClient::SendStop(const tPacketStop& packet)
{
}

void InnoOJTClient::SendStart(const tPacketStart& packet)
{
}

void InnoOJTClient::SendPosesSize(const tPacketPosesSize& packet)
{
}

void InnoOJTClient::SendPosesse(const tPacketPoses& packet)
{
}
