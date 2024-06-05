#include "pch.h"
#include "InnoOJTServer.h"
#include <TimeManager.h>

#define gLogListUI (static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI")))

static std::mutex gClientsMutex;
static SOCKET gListenSocket = INVALID_SOCKET;

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

	for (int i = 0; i < INNO_MAX_ROOM_USER; ++i)
	{
		mRoom.posesArray[i].reserve(1000000);
	}
}

InnoOJTServer::~InnoOJTServer()
{
}

// Ŭ���̾�Ʈ �ڵ鸵 �Լ�
static void handleClient(SOCKET clientSocket) 
{
	char recvbuf[INNO_MAX_PACKET_SIZE];
	int recvbuflen = INNO_MAX_PACKET_SIZE;

	InnoOJTServer* server = InnoOJTServer::GetInstance();
	LogListUI* logList = server->mLogListUI;

	while (true)
	{
		int bytesReceived = recv(clientSocket, recvbuf, recvbuflen, 0);
		std::lock_guard<std::mutex> guard(gClientsMutex);

		int packetId = getPacketId(recvbuf, recvbuflen);
		int clientID = server->GetInncoClient(clientSocket).ClientID;

		if (bytesReceived > 0)
		{
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
			case Stop:
			{
				tPacketStop stop;
				deserializeData(recvbuf, sizeof(stop), &stop);
				server->ReciveStop(clientID, stop);
			}
			break;
			default:
				logList->WriteError("Invalied packet");
				break;
			}
		}
		else if (bytesReceived == 0)
		{
			// Ŭ���̾�Ʈ ������ ����Ʈ���� ����			
			server->Disconnect(clientSocket);
			logList->WriteLine("Disconnect Client");
		}
		else
		{
			logList->WriteError("Invalied Socket");
		}

	}	
}

static void handleAccept()
{
	LogListUI* logList = static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI"));

	// Ŭ���̾�Ʈ ���� �� ó��
	while (gListenSocket != INVALID_SOCKET)
	{
		SOCKET ClientSocket = accept(gListenSocket, NULL, NULL);

		if (ClientSocket == INVALID_SOCKET)
		{
			logList->WriteError("Accept failed.");
			closesocket(gListenSocket);
			WSACleanup();
		}

		{
			std::lock_guard<std::mutex> guard(gClientsMutex);
			InnoOJTServer::GetInstance()->Accept(ClientSocket);
		}

		// ���ο� Ŭ���̾�Ʈ�� ó���ϴ� ������ ����
		std::thread(handleClient, ClientSocket).detach();
	}
}

void InnoOJTServer::run()
{
	if (gListenSocket == INVALID_SOCKET)
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
	WSADATA wsaData;
	sockaddr_in serverAddr;

	//�̹� Listen ���̶��
	if (gListenSocket != INVALID_SOCKET)
	{
		gLogListUI->WriteWarning("Already Listening.");
		return E_FAIL;
	}

	// Winsock �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		gLogListUI->WriteError("WSAStartup failed.");
		return E_FAIL;
	}

	// ���� ����
	gListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (gListenSocket == INVALID_SOCKET)
	{
		gLogListUI->WriteError("Socket creation failed.");
		WSACleanup();
		return E_FAIL;
	}

	// ���� �ּ� �� ��Ʈ ����
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(INNO_DEFAULT_PORT);

	// ������ ���� �ּҿ� ���ε�
	if (bind(gListenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		gLogListUI->WriteError("Bind failed.");
		closesocket(gListenSocket);
		WSACleanup();

		return E_FAIL;
	}

	// ListenSocket�� listen ���·θ���
	if (listen(gListenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		gLogListUI->WriteError("Listen failed");
		closesocket(gListenSocket);
		WSACleanup();
		return E_FAIL;
	}

	char buf[256] = { 0, };
	sprintf_s(buf, "Server is listening on port %d", port);
	gLogListUI->WriteLine(buf);

	std::thread accept(handleAccept);
	accept.detach();

	return S_OK;
}

int InnoOJTServer::Accept(SOCKET ClientSocket)
{
	//accept	
	AddClient(ClientSocket);

	//�ڵ����� ä�ο� �ִ´�
	mChannel.clients.push_back(GetInncoClient(ClientSocket));	
	return 0;
}

void InnoOJTServer::EnterRoom(int clientID)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);

	if (mRoom.clients.size() >= 2)
	{
		mLogListUI->WriteWarning("Room Clientes 2");
		return;
	}

	std::vector<tInnoClient>::iterator iter = mRoom.clients.begin();
	for (; iter != mRoom.clients.end(); ++iter)
	{
		if (iter->ClientID == clientID)
		{
			mLogListUI->WriteWarning("He's Already Room");
			return;
		}
	}

	tInnoClient client = GetInncoClient(clientID);
	mLogListUI->WriteLine("Enter Room");
	mRoom.clients.push_back(client);
}

void InnoOJTServer::ExitRoom(int clientID)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);

	std::vector<tInnoClient>::iterator iter = mRoom.clients.begin();
	for (; iter != mRoom.clients.end(); ++iter)
	{
		if (iter->ClientID == clientID)
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
	std::lock_guard<std::mutex> guard(gClientsMutex);
	tInnoClient client = GetInncoClient(clientID);

	send_log(client.Socket, messageLen, message);
}

void InnoOJTServer::SendPos(int clientID, float pos)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);
	tInnoClient client = GetInncoClient(clientID);

	send_pos(client.Socket, pos);
}

void InnoOJTServer::SendStop(int clientID)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);
	tInnoClient client = GetInncoClient(clientID);

	send_stop(client.Socket);
}

void InnoOJTServer::SendStart(int clientID)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);
	tInnoClient client = GetInncoClient(clientID);

	send_start(client.Socket);
}

void InnoOJTServer::SendPosesSize(int clientID, int size)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);
	tInnoClient client = GetInncoClient(clientID);

	send_poses_size(client.Socket, size);
}

void InnoOJTServer::SendPoses(int clientID, int size, const float* poses)
{
	std::lock_guard<std::mutex> guard(gClientsMutex);
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

void InnoOJTServer::ReciveStop(int clientID, const tPacketStop& outPacket)
{
	for (int i = 0; i < mRoom.clients.size(); ++i)
	{
		if (clientID == mRoom.clients[i].ClientID)
		{
			mRoom.bStop[i] = true;
			break;
		}
	}

	//��ΰ� ������ ������ �Ϸ��ߴٸ�
	int bStopCount = 0;
	for (int i = 0; i < mRoom.clients.size(); i++)
	{
		if (mRoom.bStop[i])
		{
			++bStopCount;
		}
	}
	if (bStopCount != mRoom.clients.size())
	{
		return;
	}

	//�� ��ġ������ BroadCast ���ش�.
	for (int i = 0; i < mRoom.clients.size(); ++i)
	{
		for (int j = 0; j < mRoom.clients.size(); ++j)
		{
			if (i == j)
			{
				continue;
			}

			//i��° Ŭ���̾�Ʈ�� �����͸� j������ŭ ��ε�ĳ��Ʈ�Ѵ�.
			tInnoClient client = GetInncoClient(mRoom.clients[i].ClientID);
			send_poses_size(client.Socket, 11);

			int count = mRoom.posesArray[i].size();
			std::queue<float> vecPoses;

			for (int k = 0; k < count; ++k)
			{
				vecPoses.push(mRoom.posesArray[i][k]);
			}

			//INNO_MAX_POS_SIZE ��ŭ ��� ������.
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

				send_poses(client.Socket, tempPoses.size(), tempPoses.data());
			}
		}
	}

	//�����ġ������ ��ε�ĳ��Ʈ�ߴ�. ���� ���Ḧ�˸�
	for (int i = 0; i < mRoom.clients.size(); ++i)
	{
		send_finish(mRoom.clients[i].Socket);
	}

	gLogListUI->WriteLine("Training Finish");
}

void InnoOJTServer::RecivePosesSize(int clientID, const tPacketPosesSize& outPacket)
{
	for (int i = 0; i < mRoom.clients.size(); ++i)
	{
		if (clientID == mRoom.clients[i].ClientID)
		{
			mRoom.posSize[i] = outPacket.Size;
			break;
		}
	}
}

void InnoOJTServer::RecivePoses(int clientID, const tPacketPoses& outPacket)
{
	for (int i = 0; i < mRoom.clients.size(); ++i)
	{
		if (clientID == mRoom.clients[i].ClientID)
		{
			for (int j = 0; j < outPacket.Size; ++j)
			{
				mRoom.posesArray[i].push_back(outPacket.Poses[j]);
			}
			break;
		}
	}
}

std::string InnoOJTServer::GetClientIP(SOCKET clientSocket)
{
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

void InnoOJTServer::AddClient(SOCKET socket)
{
	char buff[256] = { 0, };
	sprintf_s(buff, "%s %s", GetClientIP(socket).c_str(), "Enter");
	mLogListUI->WriteLine(buff);

	std::string ip(buff);
	mClients.push_back({ InnoOJTServer::serializeNumber++, ip, socket });

}

void InnoOJTServer::Disconnect(SOCKET socket)
{
	std::vector<tInnoClient>::iterator iter = mClients.begin();
	for (; iter != mClients.end(); ++iter)
	{
		if (iter->Socket == socket)
		{
			mClients.erase(iter);
			break;
		}
	}
	closesocket(socket);
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

tInnoClient InnoOJTServer::GetInncoClient(int clientID)
{
	std::vector<tInnoClient>::iterator iter = mClients.begin();
	for (; iter != mClients.end(); ++iter)
	{
		if (iter->ClientID == clientID)
		{
			return *iter;
		}
	}

	assert(false);
}
