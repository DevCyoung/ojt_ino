#include "pch.h"
#include "ChannelUI.h"
#include "InnoOJTServer.h"

ChannelUI::ChannelUI()
{
	SetTitle("ChannelUI");
}	

ChannelUI::~ChannelUI()
{
}

void ChannelUI::drawForm()
{
	ImGui::Begin("ChannelUI");
	//ImGui::Text("hello ChannelUI");
	
	const std::vector<tInnoClient>& clients = InnoOJTServer::GetInstance()->mChannel.clients;

	for (auto client : clients)
	{
		char buff[256] = { 0, };
		sprintf_s(buff, "%s : %d Channel", client.IP.c_str(), client.ClientID);
		ImGui::Text(buff);
	}



	ImGui::End();
}
