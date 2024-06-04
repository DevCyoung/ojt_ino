#include "pch.h"
#include "InnoOJTServer.h"
#include <TimeManager.h>
#define PACKET_SIZE 2048

InnoOJTServer::InnoOJTServer()
	: mPanelManager(nullptr)
	, mChannelUI(nullptr)
	, mLogListUI(nullptr)
	, mListenUI(nullptr)
	, mRoom{}
{
	mPanelManager = PanelUIManager::GetInstance();
	mChannelUI = static_cast<ChannelUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("ChannelUI"));
	mLogListUI = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI"));
	mListenUI = static_cast<ListenUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("ListenUI"));

	assert(mPanelManager);
	assert(mChannelUI);
	assert(mLogListUI);
	assert(mListenUI);

	mRoom.bTraining = false;
}

InnoOJTServer::~InnoOJTServer()
{

}

//static std::vector<SOCKET> clients;
static std::mutex clients_mutex;
static SOCKET ListenSocket = INVALID_SOCKET;

#define DEFAULT_PORT 54000

std::string GetClientIP(SOCKET clientSocket) {
	sockaddr_in clientAddr;
	int addrLen = sizeof(clientAddr);

	// Ŭ���̾�Ʈ ���� �ּ� ���� ��������
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
//// ��Ŷ ��� ����ü
//struct PacketHeader {
//	int packetId;
//};

// ��Ŷ ID�� Ȯ���ϴ� �Լ�
//int getPacketId(char (&buffer)[PACKET_SIZE], int recvSize) {
//	// ���۰� ��� ũ�⺸�� ������ ��ȿ�� ��Ŷ�� �ƴ�
//	if (recvSize < sizeof(int))
//	{
//		return -1; // 0�� ��ȿ���� ���� ID�� �����մϴ�.
//	}
//
//	// ��Ŷ ������� ��Ŷ ID�� ����
//	//PacketHeader header;
//	int header = 0;
//	std::memcpy(&header, buffer, sizeof(int));
//	return header;
//}

// Ŭ���̾�Ʈ �ڵ鸵 �Լ�
static void handle_client(SOCKET client_socket) {
	char recvbuf[PACKET_SIZE];
	int recvbuflen = PACKET_SIZE;
	int result;

	InnoOJTServer* server = InnoOJTServer::GetInstance();
	LogListUI* logList = server->mLogListUI;
	
	while ((result = recv(client_socket, recvbuf, recvbuflen, 0)) > 0)
	{
		//auto start = std::chrono::high_resolution_clock::now();  // ���� �ð� ���
		//recvbuf[result] = '\0';
		//std::string message = std::string(recvbuf, result);
		////std::cout << "Received: " << message << "\n";
		//
		//// ��ε�ĳ��Ʈ �޽���
		//std::lock_guard<std::mutex> guard(clients_mutex);
		//for (SOCKET client : clients)
		//{
		//	if (client != client_socket) 
		//	{
		//		send(client, message.c_str(), message.size(), 0);
		//	}
		//}
		int packetId = getPacketId(recvbuf, recvbuflen);
		int clientID = server->GetInncoClient(client_socket).ClientID;
		std::lock_guard<std::mutex> guard(clients_mutex);
		switch (packetId)
		{
		case Log:
		{
			tPacketLog log;
			deserializeData(recvbuf, sizeof(tPacketLog), &log);
			server->ReciveLog(clientID, log);
		}
			break;
		case Pos:
		{
			tPacketPos pos;
			deserializeData(recvbuf, sizeof(tPacketPos), &pos);
			server->RecivePos(clientID, pos);
		}
			break;		
		case PosesSize:
		{
			tPacketPosesSize posesSize;
			deserializeData(recvbuf, sizeof(tPacketPosesSize), &posesSize);
			server->RecivePosesSize(clientID, posesSize);
		}
			break;
		case Poses:
		{
			tPacketPoses poses;
			deserializeData(recvbuf, sizeof(tPacketPoses), &poses);
			server->RecivePoses(clientID, poses);
		}
			break;
		default:			
			logList->WriteLine("invalie packet");
			break;
		}
	}

	// Ŭ���̾�Ʈ ������ ����Ʈ���� ����
	//std::lock_guard<std::mutex> guard(clients_mutex);
	//clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
	//closesocket(client_socket);
	server->Disconnect(client_socket);
}

static void handle_accept()
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI"));

	// Ŭ���̾�Ʈ ���� �� ó��
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
		}

		// ���ο� Ŭ���̾�Ʈ�� ó���ϴ� ������ ����
		std::thread(handle_client, ClientSocket).detach();		
	}
}

void InnoOJTServer::run()
{
	if (ListenSocket == INVALID_SOCKET)
	{
		return;
	}

	static float trainingTime = 0.f;

	trainingTime += gDeltaTime;

	if (mRoom.bTraining && trainingTime >= (1.f / 120.f))
	{
		//position broadcast
		for (int i = 0; i < mRoom.clients.size(); ++i)
		{
			//broad cast

			for (int j = 0; j < mRoom.clients.size(); ++j)
			{
				if (i == j)
				{
					continue;
				}

				SendPos(mRoom.clients[i].ClientID, mRoom.poses[j]);
			}
		}


		trainingTime = 0.f;
	}
}

int InnoOJTServer::Listen(const int port)
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI"));
	assert(logList);

	WSADATA wsaData;
	sockaddr_in serverAddr;

	// Winsock �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	{
		logList->WriteLine("WSAStartup failed.");
		return 1;
	}

	// ���� ����
	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET) 
	{
		logList->WriteLine("Socket creation failed.");
		WSACleanup();
		return 1;
	}

	// ���� �ּ� �� ��Ʈ ����
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(DEFAULT_PORT);

	// ������ ���� �ּҿ� ���ε�
	if (bind(ListenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
	{
		logList->WriteLine("Bind failed.");
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	// ���� ���
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
	//accept	
	AddClient(ClientSocket);


	//�ڵ����� ä�ο� �ִ´�
	mChannel.clients.push_back(GetInncoClient(ClientSocket));


	//mChannel->AddItemUI

	return 0;
}

void InnoOJTServer::EnterRoom(int id)
{
	std::lock_guard<std::mutex> guard(clients_mutex);

	if (mRoom.clients.size() >= 2)
	{
		mLogListUI->WriteLine("Room Clientes 2");
		return;
	}

	std::vector<tInnoClient>::iterator iter = mRoom.clients.begin();
	for (; iter != mRoom.clients.end(); ++iter)
	{
		if (iter->ClientID == id)
		{
			mLogListUI->WriteLine("He's Already Room");
			return;
		}
	}

	tInnoClient client = GetInncoClient(id);
	mLogListUI->WriteLine("Enter Room");
	mRoom.clients.push_back(client);
}

void InnoOJTServer::ExitRoom(int id)
{
	std::lock_guard<std::mutex> guard(clients_mutex);

	std::vector<tInnoClient>::iterator iter = mRoom.clients.begin();
	for (; iter != mRoom.clients.end(); ++iter)
	{
		if (iter->ClientID == id)
		{
			mRoom.clients.erase(iter);
			mLogListUI->WriteLine("Exit Room");
			return;
		}
	}

	mLogListUI->WriteLine("Not exist user");
}

void InnoOJTServer::SendLog(int clientID, int messageLen, const char* message)
{
	std::lock_guard<std::mutex> guard(clients_mutex);
	tInnoClient client = GetInncoClient(clientID);

	send_log(client.Socket, messageLen, message);
}

void InnoOJTServer::SendPos(int clientID, float pos)
{
	std::lock_guard<std::mutex> guard(clients_mutex);
	tInnoClient client = GetInncoClient(clientID);

	send_pos(client.Socket, pos);
}

void InnoOJTServer::SendStop(int clientID)
{
	std::lock_guard<std::mutex> guard(clients_mutex);
	tInnoClient client = GetInncoClient(clientID);

	send_stop(client.Socket);
}

void InnoOJTServer::SendStart(int clientID)
{
	std::lock_guard<std::mutex> guard(clients_mutex);
	tInnoClient client = GetInncoClient(clientID);

	send_start(client.Socket);
}

void InnoOJTServer::SendPosesSize(int clientID, int size)
{
	std::lock_guard<std::mutex> guard(clients_mutex);
	tInnoClient client = GetInncoClient(clientID);

	send_poses_size(client.Socket, size);
}

void InnoOJTServer::SendPoses(int clientID, int size, const float* poses)
{
	std::lock_guard<std::mutex> guard(clients_mutex);
	tInnoClient client = GetInncoClient(clientID);

	send_poses(client.Socket, size, poses);
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
			mRoom.poses[i] = outPacket.Position;
			break;
		}
	}	
}

void InnoOJTServer::RecivePosesSize(int clientID, const tPacketPosesSize& outPacket)
{
}

void InnoOJTServer::RecivePoses(int clientID, const tPacketPoses& outPacket)
{
}

tInnoClient InnoOJTServer::GetInncoClient(SOCKET socket)
{
	std::vector<tInnoClient>::iterator iter = mClients.begin();
	for (; iter != mClients.end(); ++iter)
	{
		if (iter->Socket == socket)
		{
			return *iter;
		}
	}

	assert(false);
}

tInnoClient InnoOJTServer::GetInncoClient(int id)
{
	std::vector<tInnoClient>::iterator iter = mClients.begin();
	for (; iter != mClients.end(); ++iter)
	{
		if (iter->ClientID == id)
		{
			return *iter;
		}

	}
	assert(false);
}
