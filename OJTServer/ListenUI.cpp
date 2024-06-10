#include "pch.h"
#include "ListenUI.h"

#include "LogListUI.h"
#include "PanelUIManager.h"
#include "InnoOJTServer.h"
#include <imgui_theme.h>

ListenUI::ListenUI()
{
	SetTitle("ListenUI");

	{
		ImGuiStyle newStyle = ImGuiTheme::ThemeToStyle(ImGuiTheme::ImGuiTheme_(7));
		ImGuiStyle& style = ImGui::GetStyle();
		style = newStyle;
	}

}

ListenUI::~ListenUI()
{
}

void ListenUI::drawForm()
{
	static int port = 0;

	ImGui::Begin("ListenUI");
	ImGui::Text("192.168.1.186");
	ImGui::SameLine();
	//ImGui::PushStyleVar(0,0);
	ImGui::InputInt("Port ", &port);
	//ImGui::PopStyleVar();

	ImGui::SameLine();
	if (ImGui::Button("Listen"))
	{
		InnoOJTServer::GetInstance()->Listen(port);
	}

	ImGui::End();
}
