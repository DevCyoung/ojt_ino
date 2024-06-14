#include "pch.h"
#include "PanelLoader.h"
#include "ClientTestUI.h"
#include "InnoInputUI.h"
#include <PanelUIManager.h>
#include <LogListUI.h>
#include <imgui_theme.h>
#include <implot.h>

PanelLoader::PanelLoader()
{
	LogListUI* logListUI = new LogListUI();
	logListUI->SetTitle("LogListUIClient");
	PanelUIManager::GetInstance()->AddPanel(logListUI);
	PanelUIManager::GetInstance()->AddPanel(new InnoInputUI);
	PanelUIManager::GetInstance()->AddPanel(new ClientTestUI);

	ImGuiStyle newStyle = ImGuiTheme::ThemeToStyle(ImGuiTheme::ImGuiTheme_::ImGuiTheme_MicrosoftStyle);
	ImGui::GetStyle() = newStyle;

	ImPlot::CreateContext();
}

PanelLoader::~PanelLoader()
{
	ImPlot::DestroyContext();

}