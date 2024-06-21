#include "pch.h"
#include "ChannelUI.h"
#include "InnoOJTServer.h"
#include <imgui_internal.h>
ChannelUI::ChannelUI()
{
	SetTitle("ChannelUI");
}	

ChannelUI::~ChannelUI()
{
}

void ChannelUI::drawForm()
{
	ImGuiWindowClass window_GraphUI1;
	window_GraphUI1.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_GraphUI1);
	ImGui::Begin("ChannelUI");
	//ImGui::Text("hello ChannelUI");
	
	const std::vector<tInnoClient>& clients = InnoOJTServer::GetInstance()->mChannel.clients;

	for (auto client : clients)
	{
		char buff[256] = { 0, };
		sprintf_s(buff, "%s : %d Channel", client.Name.c_str(), client.ClientID);
		ImGui::Text(buff);
	}



	ImGui::End();
}
