#pragma once
#include <PanelUI.h>

class RoomUI : public PanelUI
{
public:
	RoomUI();
	~RoomUI();

	// PanelUI을(를) 통해 상속됨
	void drawForm() override;
};

