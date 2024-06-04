#include "pch.h"
#include "ClientTestUI.h"
#include <iostream>
#include "InnoOJTClient.h"
#define DEFAULT_PORT 54000
#define DEFAULT_BUFLEN 512

ClientTestUI::ClientTestUI()
{
	SetTitle("ClientTestUI");
}

ClientTestUI::~ClientTestUI()
{
}

// 서버로부터 메시지 수신하는 함수
void receive_messages(SOCKET ConnectSocket) {
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


void ClientTestUI::drawForm()
{
	ImGui::Begin("ClientTestUI");
	ImGui::Text("hello world!");

	static bool bConnect = false;

	if (!bConnect && ImGui::Button("Connect"))
	{
		InnoOJTClient::GetInstance()->Connect("127.0.0.1", 5400);
		bConnect = true;
	}

	static char buff[256] = { 0, };

	ImGui::InputText("##asd", buff, 256);

	if (ImGui::Button("test log"))
	{
		tPacketLog log = {};
		log.PacketID = ePacketID::Log;
		log.MessageLen = (int)(strlen(buff));
		//ZeroMemory(log.Message, sizeof(tPacketLog));
		memcpy(log.Message, buff, log.MessageLen);
		InnoOJTClient::GetInstance()->SendLog(log);
	}

	if (ImGui::Button("test Pos"))
	{
		tPacketPos pos = {};
		pos.PacketID = ePacketID::Pos;
		pos.Position = 4.6534f;
		//log.MessageLen = (int)(strlen(buff));
		//ZeroMemory(log.Message, sizeof(tPacketLog));
		//memcpy(pos.Message, buff, pos.MessageLen);
		InnoOJTClient::GetInstance()->SendPos(pos);
	}

	ImGui::End();
}
