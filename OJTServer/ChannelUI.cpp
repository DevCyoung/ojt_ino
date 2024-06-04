#include "pch.h"
#include "ChannelUI.h"

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
	ImGui::Text("hello ChannelUI");
	ImGui::End();
}
