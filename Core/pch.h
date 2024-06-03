#ifndef PCH_H
#define PCH_H

#include "InnoOJTCore.h"

#endif


//OJT Server
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <iostream>
//#include <thread>
//#include <vector>
//#include <mutex>
//#include <string>
//#include <chrono>  
//#pragma comment(lib, "ws2_32.lib")
//
//#define DEFAULT_PORT 54000
//
//std::vector<SOCKET> clients;
//std::mutex clients_mutex;
//
//// 클라이언트 핸들링 함수
//void handle_client(SOCKET client_socket) {
//	char recvbuf[512];
//	int recvbuflen = 512;
//	int result;
//
//	while ((result = recv(client_socket, recvbuf, recvbuflen, 0)) > 0) {
//		auto start = std::chrono::high_resolution_clock::now();  // 시작 시간 기록
//		recvbuf[result] = '\0';
//		std::string message = std::string(recvbuf, result);
//		std::cout << "Received: " << message << "\n";
//
//		// 브로드캐스트 메시지
//		std::lock_guard<std::mutex> guard(clients_mutex);
//		for (SOCKET client : clients) {
//			if (client != client_socket) {
//				send(client, message.c_str(), message.size(), 0);
//			}
//		}
//		auto end = std::chrono::high_resolution_clock::now();  // 종료 시간 기록
//		std::chrono::duration<double> elapsed = end - start;
//		std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";  // 경과 시간 출력
//	}
//
//	// 클라이언트 소켓을 리스트에서 제거
//	std::lock_guard<std::mutex> guard(clients_mutex);
//	clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
//	closesocket(client_socket);
//}
//
//int main() {
//	WSADATA wsaData;
//	SOCKET ListenSocket = INVALID_SOCKET;
//	sockaddr_in serverAddr;
//
//	// Winsock 초기화
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		std::cerr << "WSAStartup failed.\n";
//		return 1;
//	}
//
//	// 소켓 생성
//	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (ListenSocket == INVALID_SOCKET) {
//		std::cerr << "Socket creation failed.\n";
//		WSACleanup();
//		return 1;
//	}
//
//	// 서버 주소 및 포트 설정
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = INADDR_ANY;
//	serverAddr.sin_port = htons(DEFAULT_PORT);
//
//	// 소켓을 서버 주소와 바인딩
//	if (bind(ListenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
//		std::cerr << "Bind failed.\n";
//		closesocket(ListenSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	// 연결 대기
//	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
//		std::cerr << "Listen failed.\n";
//		closesocket(ListenSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	std::cout << "Server is listening on port " << DEFAULT_PORT << "\n";
//
//	// 클라이언트 연결 및 처리
//	while (true) {
//		SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
//		if (ClientSocket == INVALID_SOCKET) {
//			std::cerr << "Accept failed.\n";
//			closesocket(ListenSocket);
//			WSACleanup();
//			return 1;
//		}
//
//		{
//			std::lock_guard<std::mutex> guard(clients_mutex);
//			clients.push_back(ClientSocket);
//		}
//
//		// 새로운 클라이언트를 처리하는 쓰레드 시작
//		std::thread(handle_client, ClientSocket).detach();
//		std::cout << "Client connected.\n";
//	}
//
//	// 클린업
//	closesocket(ListenSocket);
//	WSACleanup();
//	return 0;
//}



//OJT Client
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <iostream>
//#include <thread>
//#include <string>
//#include <chrono>  
//#pragma comment(lib, "ws2_32.lib")
//
//#define DEFAULT_PORT 54000
//#define DEFAULT_BUFLEN 512
//
//// 서버로부터 메시지 수신하는 함수
//void receive_messages(SOCKET ConnectSocket) {
//	char recvbuf[DEFAULT_BUFLEN];
//	int recvbuflen = DEFAULT_BUFLEN;
//
//	while (true) {
//		auto start = std::chrono::high_resolution_clock::now();  // 시작 시간 기록
//		int bytesReceived = recv(ConnectSocket, recvbuf, recvbuflen, 0);
//		auto end = std::chrono::high_resolution_clock::now();  // 종료 시간 기록
//		if (bytesReceived > 0) {
//			std::cout << "Received: " << std::string(recvbuf, 0, bytesReceived) << "\n";
//			std::chrono::duration<double> elapsed = end - start;
//		}
//		else if (bytesReceived == 0) {
//			std::cout << "Connection closed by server.\n";
//			break;
//		}
//		else {
//			std::cerr << "recv failed.\n";
//			break;
//		}
//	}
//
//	closesocket(ConnectSocket);
//	WSACleanup();
//}
//
//int main() {
//	WSADATA wsaData;
//	SOCKET ConnectSocket = INVALID_SOCKET;
//	sockaddr_in clientService;
//
//	// Winsock 초기화
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		std::cerr << "WSAStartup failed.\n";
//		return 1;
//	}
//
//	// 소켓 생성
//	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (ConnectSocket == INVALID_SOCKET) {
//		std::cerr << "Socket creation failed.\n";
//		WSACleanup();
//		return 1;
//	}
//
//	// 서버 주소 설정
//	clientService.sin_family = AF_INET;
//	clientService.sin_port = htons(DEFAULT_PORT);
//
//	// 서버에 연결
//	if (inet_pton(AF_INET, "127.0.0.1", &clientService.sin_addr) <= 0) {
//		std::cerr << "Invalid address/ Address not supported \n";
//		closesocket(ConnectSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	if (connect(ConnectSocket, (sockaddr*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
//		std::cerr << "Connect failed.\n";
//		closesocket(ConnectSocket);	
//		WSACleanup();
//		return 1;
//	}
//
//	std::cout << "Connected to server.\n";
//
//	// 서버로부터 메시지를 받는 쓰레드 시작
//	std::thread(receive_messages, ConnectSocket).detach();
//
//	std::string sendbuf;
//	while (true) {
//		std::getline(std::cin, sendbuf);
//
//		auto start = std::chrono::high_resolution_clock::now();  // 시작 시간 기록
//		if (send(ConnectSocket, sendbuf.c_str(), sendbuf.length(), 0) == SOCKET_ERROR) {
//			std::cerr << "Send failed.\n";
//			break;
//		}
//		auto end = std::chrono::high_resolution_clock::now();  // 종료 시간 기록
//		std::chrono::duration<double> elapsed = end - start;
//	}
//
//	closesocket(ConnectSocket);
//	WSACleanup();
//	return 0;
//}
