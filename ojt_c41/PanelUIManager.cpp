#include "pch.h"
#include "PanelUIManager.h"
#include "PanelUI.h"




PanelUIManager::PanelUIManager()
	: mPanelUIs()
{
	mPanelUIs.reserve(100);
}

PanelUIManager::~PanelUIManager()
{
	mem::del::DeleteVectorElements(&mPanelUIs);
}

void PanelUIManager::AddPanel(PanelUI* const panelUI)
{
	Assert(panelUI, ASSERT_MSG_NULL);

	mPanelUIs.push_back(panelUI);
}

PanelUI* PanelUIManager::FindPanelUIOrNull(const std::string& title)
{
	PanelUI* resultPanelUI = nullptr;

	for (PanelUI* const panelUI : mPanelUIs)
	{
		if (panelUI->GetTitle() == title)
		{
			resultPanelUI = panelUI;
			break;
		}
	}

	return resultPanelUI;
}

void PanelUIManager::update()
{
	for (PanelUI* panelUI : mPanelUIs)
	{
		panelUI->update();
	}
}

void PanelUIManager::finalUpdate()
{
	for (PanelUI* panelUI : mPanelUIs)
	{
		panelUI->finalUpdate();
	}
}

void PanelUIManager::render()
{
	for (PanelUI* panelUI : mPanelUIs)
	{
		panelUI->render();
	}
}
