#include "pch.h"
#include "LogListUI.h"
#include "imgui_internal.h"
std::mutex WriteMutex;

LogListUI::LogListUI()
{
	SetTitle("LogListUI");

	mLogs.reserve(30000);
}

LogListUI::~LogListUI()
{	
}

void LogListUI::drawForm()
{
	ImGuiWindowClass window_LogListUI;
	window_LogListUI.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_LogListUI);
	ImGui::Begin(GetTitle().c_str());
	std::lock_guard guard(WriteMutex);

	static int count = mLogs.size();

	

	for (auto& log: mLogs)
	{
		if (log.Type == eInnoMessageType::Default)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));			
			
		}		
		else if (log.Type == eInnoMessageType::Error)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 50, 50, 255));			
		}
		else if (log.Type == eInnoMessageType::Warning)
		{
			ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 50, 255));			
		}
		else
		{
			assert(false);
		}
		ImGui::Text(log.Message.c_str());
		ImGui::PopStyleColor();
	}

	if (count != mLogs.size())
	{
		ImGui::SetScrollHereY();

		count = mLogs.size();
	}

	ImGui::End();
}

void LogListUI::WriteLine(const std::string& message)
{
	std::lock_guard guard(WriteMutex);
	mLogs.push_back({eInnoMessageType::Default, message });
}

void LogListUI::WriteError(const std::string& message)
{
	std::string errorMessage = "Error: ";
	errorMessage += message;

	std::lock_guard guard(WriteMutex);	
	mLogs.push_back({ eInnoMessageType::Error,errorMessage });
}

void LogListUI::WriteWarning(const std::string& message)
{
	std::string warningMessage = "Warning: ";
	warningMessage += message;

	std::lock_guard guard(WriteMutex);
	mLogs.push_back({ eInnoMessageType::Warning,warningMessage });
}
