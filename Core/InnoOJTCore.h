#ifndef PCH_H
#define PCH_H
//#include <Helper\Helper.h>

#include <assert.h>
#include <Windows.h>
#include <string>
#include <vector> 
#include <stack>
#include <list> 
#include <map> 
#include <queue>
#include <unordered_map> 
#include <bitset> 
#include <set> 
#include <functional> 
#include <cmath> 
#include <algorithm> 
#include <limits>
#include <memory> 
#include <filesystem> 
#include <wrl.h>
#include <commdlg.h>
#include <d3d11.h>

#include "BuildSetting.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_helper.h"
#include "StringHelper.h"
#include "DXMath.h"
#include "DirectXTex.h"


using namespace engine::math;

#define DELETE_POINTER(p)		{ delete   p; p = nullptr; }
#define DELETE_ARRAY(p)			{ delete[] p; p = nullptr; }

#define DELETE_POINTER_NOT_NULL(p)  { Assert(p, ASSERT_MSG_NULL); DELETE_POINTER(p) }
#define DELETE_ARRAY_NOT_NULL(p)	{ Assert(p, ASSERT_MSG_NULL); DELETE_ARRAY(p) }

namespace mem
{
	template<typename T, UINT Size>
	void DeletePointerArrayElements(T* (* const pPointerArray)[Size])
	{
		for (auto& value : (*pPointerArray))
		{
			DELETE_POINTER(value);
		}
	}

	template<typename T>
	void DeleteMapElements(T* const pMap)
	{
		for (auto& value : (*pMap))
		{
			DELETE_POINTER(value.second);
		}

		pMap->clear();
	}

	template<typename T>
	void DeleteContainerElements(T* const pContainer)
	{
		for (auto& value : (*pContainer))
		{
			DELETE_POINTER(value);
		}

		pContainer->clear();
	}
}

namespace mem::del
{
	template<typename T, UINT Size>
	void DeletePointerArrayElements(T* (* const pPointerArray)[Size])
	{
		mem::DeletePointerArrayElements(pPointerArray);
	}

	template<typename Pointer>
	void DeleteVectorElements(std::vector<Pointer>* const pVector)
	{
		mem::DeleteContainerElements(pVector);
	}

	template<typename Key, typename Pointer>
	void DeleteMapElements(std::map<Key, Pointer>* const map)
	{
		mem::DeleteMapElements(map);
	}

	template<typename Key, typename Pointer>
	void DeleteUnorderedMapElements(std::unordered_map<Key, Pointer>* const pUnorderedMap)
	{
		mem::DeleteMapElements(pUnorderedMap);
	}

	template<typename Pointer, UINT Size>
	void DeleteVectorArrayElements(std::vector<Pointer>(* const pVectorArray)[Size])
	{
		for (auto& value : (*pVectorArray))
		{
			DeleteVectorElements(&value);
		}
	}

	template<typename Key, typename Pointer, UINT Size>
	void DeleteMapArrayElements(std::map<Key, Pointer>(* const pMapArray)[Size])
	{
		for (auto& value : (*pMapArray))
		{
			DeleteMapElements(&value);
		}
	}

	template<typename Key, typename Pointer, UINT Size>
	void DeleteUnorderedMapArrayElements(std::unordered_map<Key, Pointer>(* const pUnorderedMapArray)[Size])
	{
		for (auto& value : (*pUnorderedMapArray))
		{
			DeleteUnorderedMapElements(&value);
		}
	}
}


#include "framework.h"
#include "EnumFlag.h"

#define ASSERT_MSG_SWITCH_DEFAULT "switch default"
#define ASSERT_MSG_NULL "value is nullptr"
#define ASSERT_MSG_NOT_NULL "value is not nullptr"
#define ASSERT_MSG_INVALID "value is invalid"

#define L_PREFIX(quote) L##quote
#define ASSERT_MSG(message) message

#define Assert(expression, message) (void)(expression); assert(expression && L_PREFIX(message))
#define Static_Assert(expression, message) static_assert(expression, message)

#define FIXME(message) MessageBox(nullptr, L_PREFIX(message), L_PREFIX(message), MB_OK | MB_ICONWARNING)

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }



#define SINGLETON_DECLARE(CLASSNAME)						\
private:													\
	static void initialize()								\
	{														\
		Assert(!sInstance, ASSERT_MSG_NOT_NULL);			\
		sInstance = new CLASSNAME();						\
	}														\
static void deleteInstance()								\
	{														\
		Assert(sInstance, ASSERT_MSG_NULL);				\
		delete sInstance;									\
		sInstance = nullptr;								\
	}														\
public:														\
	static CLASSNAME* GetInstance()							\
	{														\
		Assert(sInstance, ASSERT_MSG_NULL);				\
		return sInstance;									\
	}														\
private:													\
	CLASSNAME();											\
	virtual ~CLASSNAME();									\
	CLASSNAME(const CLASSNAME&) = delete;					\
	CLASSNAME& operator=(const CLASSNAME&) = delete;		\
private:													\
	inline static CLASSNAME* sInstance = nullptr;			\


#define PUBLIC_SINGLETON_DECLARE(CLASSNAME)					\
public:														\
	static void initialize()								\
	{														\
		Assert(!sInstance, ASSERT_MSG_NOT_NULL);			\
		sInstance = new CLASSNAME();						\
	}														\
static void deleteInstance()								\
	{														\
		Assert(sInstance, ASSERT_MSG_NULL);				\
		delete sInstance;									\
		sInstance = nullptr;								\
	}														\
public:														\
	static CLASSNAME* GetInstance()							\
	{														\
		Assert(sInstance, ASSERT_MSG_NULL);				\
		return sInstance;									\
	}														\
private:													\
	CLASSNAME();											\
	virtual ~CLASSNAME();									\
	CLASSNAME(const CLASSNAME&) = delete;					\
	CLASSNAME& operator=(const CLASSNAME&) = delete;		\
private:													\
	inline static CLASSNAME* sInstance = nullptr;			\


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
//// Ŭ���̾�Ʈ �ڵ鸵 �Լ�
//void handle_client(SOCKET client_socket) {
//	char recvbuf[512];
//	int recvbuflen = 512;
//	int result;
//
//	while ((result = recv(client_socket, recvbuf, recvbuflen, 0)) > 0) {
//		auto start = std::chrono::high_resolution_clock::now();  // ���� �ð� ���
//		recvbuf[result] = '\0';
//		std::string message = std::string(recvbuf, result);
//		std::cout << "Received: " << message << "\n";
//
//		// ��ε�ĳ��Ʈ �޽���
//		std::lock_guard<std::mutex> guard(clients_mutex);
//		for (SOCKET client : clients) {
//			if (client != client_socket) {
//				send(client, message.c_str(), message.size(), 0);
//			}
//		}
//		auto end = std::chrono::high_resolution_clock::now();  // ���� �ð� ���
//		std::chrono::duration<double> elapsed = end - start;
//		std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";  // ��� �ð� ���
//	}
//
//	// Ŭ���̾�Ʈ ������ ����Ʈ���� ����
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
//	// Winsock �ʱ�ȭ
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		std::cerr << "WSAStartup failed.\n";
//		return 1;
//	}
//
//	// ���� ����
//	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (ListenSocket == INVALID_SOCKET) {
//		std::cerr << "Socket creation failed.\n";
//		WSACleanup();
//		return 1;
//	}
//
//	// ���� �ּ� �� ��Ʈ ����
//	serverAddr.sin_family = AF_INET;
//	serverAddr.sin_addr.s_addr = INADDR_ANY;
//	serverAddr.sin_port = htons(DEFAULT_PORT);
//
//	// ������ ���� �ּҿ� ���ε�
//	if (bind(ListenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
//		std::cerr << "Bind failed.\n";
//		closesocket(ListenSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	// ���� ���
//	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
//		std::cerr << "Listen failed.\n";
//		closesocket(ListenSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	std::cout << "Server is listening on port " << DEFAULT_PORT << "\n";
//
//	// Ŭ���̾�Ʈ ���� �� ó��
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
//		// ���ο� Ŭ���̾�Ʈ�� ó���ϴ� ������ ����
//		std::thread(handle_client, ClientSocket).detach();
//		std::cout << "Client connected.\n";
//	}
//
//	// Ŭ����
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
//// �����κ��� �޽��� �����ϴ� �Լ�
//void receive_messages(SOCKET ConnectSocket) {
//	char recvbuf[DEFAULT_BUFLEN];
//	int recvbuflen = DEFAULT_BUFLEN;
//
//	while (true) {
//		auto start = std::chrono::high_resolution_clock::now();  // ���� �ð� ���
//		int bytesReceived = recv(ConnectSocket, recvbuf, recvbuflen, 0);
//		auto end = std::chrono::high_resolution_clock::now();  // ���� �ð� ���
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
//	// Winsock �ʱ�ȭ
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//		std::cerr << "WSAStartup failed.\n";
//		return 1;
//	}
//
//	// ���� ����
//	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (ConnectSocket == INVALID_SOCKET) {
//		std::cerr << "Socket creation failed.\n";
//		WSACleanup();
//		return 1;
//	}
//
//	// ���� �ּ� ����
//	clientService.sin_family = AF_INET;
//	clientService.sin_port = htons(DEFAULT_PORT);
//
//	// ������ ����
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
//	// �����κ��� �޽����� �޴� ������ ����
//	std::thread(receive_messages, ConnectSocket).detach();
//
//	std::string sendbuf;
//	while (true) {
//		std::getline(std::cin, sendbuf);
//
//		auto start = std::chrono::high_resolution_clock::now();  // ���� �ð� ���
//		if (send(ConnectSocket, sendbuf.c_str(), sendbuf.length(), 0) == SOCKET_ERROR) {
//			std::cerr << "Send failed.\n";
//			break;
//		}
//		auto end = std::chrono::high_resolution_clock::now();  // ���� �ð� ���
//		std::chrono::duration<double> elapsed = end - start;
//	}
//
//	closesocket(ConnectSocket);
//	WSACleanup();
//	return 0;
//}
