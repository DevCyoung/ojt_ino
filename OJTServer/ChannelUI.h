#pragma once
#include <PanelUI.h>

class ChannelUI : public PanelUI
{
public:
	ChannelUI();
	~ChannelUI();
	// PanelUI��(��) ���� ��ӵ�
	void drawForm() override;
};

