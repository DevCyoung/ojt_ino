#include "pch.h"
#include "ChannelRoomMoveUI.h"
#include "InnoOJTServer.h"

ChannelRoomMoveUI::ChannelRoomMoveUI()
{
	SetTitle("ChannelRoomMoveUI");
}

ChannelRoomMoveUI::~ChannelRoomMoveUI()
{
}

void ChannelRoomMoveUI::drawForm()
{
	ImGui::Begin("ChannelRoomMoveUI");
	ImGui::Text("hello ChannelRoomMoveUI");

	InnoOJTServer* server = InnoOJTServer::GetInstance();

	static int item = 0;
	
	ImGui::InputInt("id", &item);

	if (ImGui::Button(">>"))
	{
		server->EnterRoom(item);
	}

	ImGui::SameLine();

	if (ImGui::Button("<<"))
	{
		server->ExitRoom(item);
	}

	ImGui::End();
}
