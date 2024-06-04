#pragma once

#include "PanelUI.h"

class LogListUI : public PanelUI
{
public:
	LogListUI();
	~LogListUI();
	// PanelUI을(를) 통해 상속됨
	void drawForm() override;

	void WriteLine(const std::string& message);

private:
	std::vector<std::string> mLogs;
};
