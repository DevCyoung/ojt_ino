#pragma once
#include "PanelUI.h"

class ClientTestUI : public PanelUI
{
public:
	ClientTestUI();
	~ClientTestUI();
	// PanelUI��(��) ���� ��ӵ�
	void drawForm() override;
};

