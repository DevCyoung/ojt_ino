#pragma once
#include "PanelUI.h"
class GameObject;
class EditorSetting : public PanelUI
{
public:
	EditorSetting();
	virtual ~EditorSetting();
	static GameObject* CreateObject();
	static float SkeletonScale;

private:
	// PanelUI��(��) ���� ��ӵ�
	
	void drawForm() override;	
};

