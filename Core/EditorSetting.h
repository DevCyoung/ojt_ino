#pragma once
#include "PanelUI.h"
#include "CarSimulation.h"

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
	CarSimulation mCarSumulation;
	void drawForm() override;	
};

