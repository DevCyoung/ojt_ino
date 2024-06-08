#pragma once
#include <PanelUI.h>

#include <imgui_theme.h>
#include <Texture.h>
#include <ResourceManager.h>
#include <Engine.h>
#include <GraphicDeviceDx11.h>
#include <GameObject.h>
#include <Builder.h>
#include <Camera.h>
#include <CameraInputMoveMent.h>
#include <Transform.h>

class InnoInputUI : public PanelUI
{
public:
	InnoInputUI();
	~InnoInputUI();
	// PanelUI��(��) ���� ��ӵ�
	void drawForm() override;

	GameObject* mEditorCamera;
};

