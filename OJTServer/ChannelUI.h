#pragma once
#include <PanelUI.h>

class ChannelUI : public PanelUI
{
public:
	ChannelUI();
	~ChannelUI();
	// PanelUI을(를) 통해 상속됨
	void drawForm() override;
};

