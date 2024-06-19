#include "pch.h"
#include "TrainingUI.h"
#include "InnoOJTServer.h"

#define gLogListUI (static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI")))

TrainingUI::TrainingUI()
{
	SetTitle("TrainingUI");
}

TrainingUI::~TrainingUI()
{
}

void TrainingUI::drawForm()
{
	
}
	