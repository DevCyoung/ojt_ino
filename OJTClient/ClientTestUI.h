#pragma once
#include "PanelUI.h"
#include "CarSimulation.h"

class ClientTestUI : public PanelUI
{
public:
	ClientTestUI();
	~ClientTestUI();
	// PanelUI��(��) ���� ��ӵ�
	void drawForm() override;

	CarSimulation mCarSumulation;
};

