#include "pch.h"
#include "EditorPanelInitialize.h"
#include "PanelUIManager.h"

#include "EditorSetting.h"
#include "TestUI2.h"
#include "ESCUI.h"
#include "EditorViewUI.h"
#include "LogUI.h"
#include "FolderViewUI.h"

void EditorPanelInitialize()
{		
	AddPanel(new EditorSetting);
	AddPanel(new TestUI2);	
	AddPanel(new ESCUI);
	AddPanel(new EditorViewUI);
	AddPanel(new LogUI);
	AddPanel(new FolderViewUI);


}

void AddPanel(PanelUI* const panelUI)
{
	Assert(panelUI, ASSERT_MSG_NULL);

	PanelUIManager::GetInstance()->AddPanel(panelUI);
}
