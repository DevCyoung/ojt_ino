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
	static int port = INNO_DEFAULT_PORT;
	constexpr float offset = 16.f;
	std::string ip = InnoOJTServer::GetInstance()->GetServerIP();

	ImGui::Begin("ListenUI");
	ImGui::Text("IP: %s  Port:", ip.c_str());
	ImGui::SameLine(175.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::InputInt("##ListenPort", &port, 0, 0);
	ImGui::SameLine();
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	
	if (ImGui::Button("Listen", ImVec2(90.f, 20.f)))
	{
		InnoOJTServer::GetInstance()->Listen(port);
	}

	ImGui::End();
}
