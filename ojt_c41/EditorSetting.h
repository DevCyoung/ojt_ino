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
	// PanelUI��(��) ���� ��ӵ�
	CarSimulation mCarSumulation;
	void drawForm() override;	
};

