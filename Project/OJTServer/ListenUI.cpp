#include "pch.h"
#include "ListenUI.h"
#include "LogListUI.h"
#include "InnoOJTServer.h"
#include <PanelUIManager.h>
#include <imgui_theme.h>
#include <imgui_internal.h>
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
	bool bServerListening = InnoOJTServer::GetInstance()->IsListening();

	ImGuiWindowClass window_GraphUI1;
	window_GraphUI1.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_GraphUI1);
	ImGui::Begin("ListenUI");

	if (bServerListening)
	{
		ImGui::Text("Listening... IP: %s Port: %d", ip.c_str(), port);
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);


		if (ImGui::BeginPopupModal("Server::DisConnect", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure disconnect?");

			ImVec2 button_size(ImGui::GetFontSize() * 7.0f, 0.0f);

			if (ImGui::Button("Yes", button_size))
			{
				InnoOJTServer::GetInstance()->DisConnect();
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("No", button_size))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::Button("DisConnect", ImVec2(90.f, 20.f)))
		{
			if (!ImGui::IsPopupOpen("Server::DisConnect"))
				ImGui::OpenPopup("Server::DisConnect");
		}
	}
	else
	{
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
	}	

	ImGui::End();
}
