#pragma once
#include "PanelUI.h"

class FolderViewUI : public PanelUI
{
public:
	FolderViewUI();
	virtual ~FolderViewUI();

	void Pop();
	void Enter(const std::string& enter);
	

private:
	std::stack<std::wstring> mCD;

	void drawForm() override;
	void update() override;
};

