#pragma once
#include <PanelUI.h>
#include <imgui_theme.h>
#include <Engine.h>

class InnoInputUI : public PanelUI
{
public:
	InnoInputUI();
	~InnoInputUI();
	// PanelUI��(��) ���� ��ӵ�
	void drawForm() override;
};

