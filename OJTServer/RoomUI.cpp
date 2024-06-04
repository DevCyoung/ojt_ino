#include "pch.h"
#include "RoomUI.h"
#include "InnoOJTServer.h"

RoomUI::RoomUI()
{
	SetTitle("RoomUI");
}

RoomUI::~RoomUI()
{
}

void RoomUI::drawForm()
{
	ImGui::Begin("RoomUI");
			
	const std::vector<tInnoClient>& channel_clients = InnoOJTServer::GetInstance()->mRoom.clients;

	for (auto client : channel_clients)
	{
		char buff[256] = { 0, };
		sprintf_s(buff, "%s : %d", client.ip.c_str(), client.id);
		ImGui::Text(buff);
	}

	ImGui::End();
}
