#pragma once
#include <PanelUI.h>

class ListenUI : public PanelUI
{
public:
	ListenUI();
	~ListenUI();
	// PanelUI을(를) 통해 상속됨
	void drawForm() override;
};

