#pragma once
#include "PanelUI.h"
class GraphUI : public PanelUI
{
public:
	GraphUI();
	~GraphUI();
	//	PanelUI을(를) 통해 상속됨
	void drawForm() override;
};

