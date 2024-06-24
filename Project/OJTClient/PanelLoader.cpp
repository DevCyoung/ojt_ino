#include "pch.h"
#include "PanelLoader.h"
#include "InnoInputUI.h"


PanelLoader::PanelLoader()
{
	LogListUI* logListUI = new LogListUI();
	logListUI->SetTitle("LogListUIClient");
	PanelUIManager::GetInstance()->AddPanel(logListUI);
	PanelUIManager::GetInstance()->AddPanel(new InnoInputUI);
}

PanelLoader::~PanelLoader()
{

}