#pragma once
#include "PanelUI.h"

class ClientTestUI : public PanelUI
{
public:
	ClientTestUI();
	~ClientTestUI();
	// PanelUI을(를) 통해 상속됨
	void drawForm() override;
};

