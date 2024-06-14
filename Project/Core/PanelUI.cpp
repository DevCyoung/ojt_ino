#include "pch.h"
#include "PanelUI.h"
#include "Editor.h"

PanelUI::PanelUI()
	: mbRender(true)
	, mTitle("NONE")
{
}

PanelUI::~PanelUI()
{
	mem::del::DeleteVectorElements(&mItemUIs);
	mem::del::DeleteVectorElements(&mChildPanels);
}

void PanelUI::render()
{
	if (false == mbRender)
	{
		return;
	}

	drawForm();	
}
