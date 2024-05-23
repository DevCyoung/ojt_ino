#pragma once

class PanelUI;

class PanelUIManager
{
	friend class Editor;
	SINGLETON_DECLARE(PanelUIManager);

public:
	void AddPanel(PanelUI* const panelUI);
	PanelUI* FindPanelUIOrNull(const std::string& title);
	void update();

private:
	void finalUpdate();
	void render();

private:
	std::vector<PanelUI*> mPanelUIs;
};

