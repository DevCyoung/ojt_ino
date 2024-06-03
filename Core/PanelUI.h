#pragma once
//#include <ImGUI/imgui.h>
//#include <ImGUI/imgui_impl_dx11.h>
//#include <ImGUI/imgui_impl_win32.h>

#include "ItemUI.h"


class PanelUI
{
	friend class PanelUIManager;

public:
	PanelUI();
	virtual ~PanelUI();

	void TurnOnVisible() { mbRender = true; }
	void TurnOffVisible() { mbRender = false; }

	const std::string& GetTitle() { return mTitle; }
	void SetTitle(const std::string& title) { mTitle = title; }

	void AddItemUI(ItemUI* const itemUI)
	{
		Assert(itemUI, ASSERT_MSG_NOT_NULL);
		mItemUIs.push_back(itemUI);
	}

	void AddPanelUI(PanelUI* const itemUI)
	{
		Assert(itemUI, ASSERT_MSG_NOT_NULL);
		mChildPanels.push_back(itemUI);
	}



private:
	virtual void update() {};
	virtual void finalUpdate() {};
	virtual void drawForm() = 0;

	void render();

	bool mbRender;
	std::string mTitle;
	std::vector<ItemUI*>	mItemUIs;
	std::vector<PanelUI*>	mChildPanels;
	
};

