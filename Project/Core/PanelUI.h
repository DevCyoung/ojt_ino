#pragma once
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
	std::vector<PanelUI*>	mChildPanels;
};

