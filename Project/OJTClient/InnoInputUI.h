#pragma once
class InnoInputUI : public PanelUI
{
public:
	InnoInputUI();
	~InnoInputUI();
	// PanelUI��(��) ���� ��ӵ�
	void drawForm() override;

	bool mbSaveClicked;
	char mName[256];
};

