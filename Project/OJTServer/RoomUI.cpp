#include "pch.h"
#include "RoomUI.h"
#include "InnoOJTServer.h"
#include <imgui_internal.h>

RoomUI::RoomUI()
{
	SetTitle("RoomUI");
}

RoomUI::~RoomUI()
{
}

void RoomUI::drawForm()
{
	ImGuiWindowClass window_GraphUI1;
	window_GraphUI1.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_GraphUI1);
	ImGui::Begin("RoomUI");
			
	const std::vector<tInnoClient>& channel_clients = InnoOJTServer::GetInstance()->mRoom.clients;

	for (auto client : channel_clients)
	{
		char buff[256] = { 0, };
		sprintf_s(buff, "%s : %d : Room", client.IP.c_str(), client.ClientID);
		ImGui::Text(buff);
	}

	ImGui::End();
}
