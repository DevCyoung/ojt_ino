#pragma once
#include <PanelUI.h>
#include <imgui_theme.h>
#include <Engine.h>

class InnoInputUI : public PanelUI
{
public:
	InnoInputUI();
	~InnoInputUI();
	// PanelUI을(를) 통해 상속됨
	void drawForm() override;
};

