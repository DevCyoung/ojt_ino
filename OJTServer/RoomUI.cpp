#include "pch.h"
#include "RoomUI.h"

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
	ImGui::Text("Hello RoomUI");
	ImGui::End();
}
