#include "pch.h"
#include "ChannelRoomMoveUI.h"

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
	ImGui::End();
}
