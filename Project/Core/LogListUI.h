#pragma once

#include "PanelUI.h"

enum class eInnoMessageType
{
	Default,
	Error,
	Warning,
};

struct tInnoMessageLog
{
	eInnoMessageType Type;
	std::string Message;
};

class LogListUI : public PanelUI
{
public:
	LogListUI();
	~LogListUI();
	// PanelUI을(를) 통해 상속됨
	void drawForm() override;

	void WriteLine(const std::string& message);
	void WriteError(const std::string& message);
	void WriteWarning(const std::string& message);

private:
	std::vector<tInnoMessageLog> mLogs;
};