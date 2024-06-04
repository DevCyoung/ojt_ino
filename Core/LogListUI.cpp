#include "pch.h"
#include "LogListUI.h"

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
	ImGui::Begin(GetTitle().c_str());
	std::lock_guard guard(WriteMutex);

	for (std::string& log: mLogs)
	{
		ImGui::Text(log.c_str());
	}	
	//sImGui::Scroll();
	ImGui::End();
}

void LogListUI::WriteLine(const std::string& message)
{
	std::lock_guard guard(WriteMutex);
	mLogs.push_back(message);
}
