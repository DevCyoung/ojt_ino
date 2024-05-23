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
	// PanelUI을(를) 통해 상속됨
	
	void drawForm() override;	
};

