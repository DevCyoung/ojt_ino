#pragma once
#include <PanelUI.h>

class ChannelRoomMoveUI : public PanelUI
{
public:
	ChannelRoomMoveUI();
	~ChannelRoomMoveUI();
	// PanelUI을(를) 통해 상속됨
	void drawForm() override;
};

