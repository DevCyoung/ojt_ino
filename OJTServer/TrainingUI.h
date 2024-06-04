#pragma once
#include <PanelUI.h>

class TrainingUI : public PanelUI
{
public:
	TrainingUI();
	~TrainingUI();
	// PanelUI을(를) 통해 상속됨
	void drawForm() override;
};

