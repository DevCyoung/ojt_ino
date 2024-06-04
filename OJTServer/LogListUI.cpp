#include "pch.h"
#include "LogListUI.h"


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
	ImGui::Begin("LogListUI");
	for (std::string& log: mLogs)
	{
		ImGui::Text(log.c_str());
	}	
	//sImGui::Scroll();
	ImGui::End();
}

void LogListUI::WriteLine(const std::string& message)
{
	mLogs.push_back(message);
}
