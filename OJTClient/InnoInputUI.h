#pragma once
#include <PanelUI.h>

class InnoInputUI : public PanelUI
{
public:
	InnoInputUI();
	~InnoInputUI();
	// PanelUI을(를) 통해 상속됨
	void drawForm() override;
};

