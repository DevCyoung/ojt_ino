#pragma once
#include <PanelUI.h>

class RoomUI : public PanelUI
{
public:
	RoomUI();
	~RoomUI();

	// PanelUI��(��) ���� ��ӵ�
	void drawForm() override;
};

