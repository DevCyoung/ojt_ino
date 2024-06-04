#pragma once

#include "PanelUI.h"

class LogListUI : public PanelUI
{
public:
	LogListUI();
	~LogListUI();
	// PanelUI��(��) ���� ��ӵ�
	void drawForm() override;

	void WriteLine(const std::string& message);

private:
	std::vector<std::string> mLogs;
};
