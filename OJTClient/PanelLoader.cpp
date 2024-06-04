#include "pch.h"
#include <PanelUIManager.h>

#include "PanelLoader.h"
//#include "ChannelRoomMoveUI.h"
//#include "ChannelUI.h"
//#include "GraphUI.h"
//#include "ListenUI.h"
//#include "LogListUI.h"
//#include "RoomUI.h"
//#include "TrainingUI.h"
#include "ClientTestUI.h"
#include <LogListUI.h>

PanelLoader::PanelLoader()
{
	//PanelUIManager::GetInstance()->AddPanel(new ChannelRoomMoveUI);
	//PanelUIManager::GetInstance()->AddPanel(new ChannelUI);
	//PanelUIManager::GetInstance()->AddPanel(new GraphUI);
	//PanelUIManager::GetInstance()->AddPanel(new ListenUI);
	//PanelUIManager::GetInstance()->AddPanel(new LogListUI);
	//PanelUIManager::GetInstance()->AddPanel(new RoomUI);
	PanelUIManager::GetInstance()->AddPanel(new ClientTestUI);

	LogListUI* logListUI = new LogListUI();
	logListUI->SetTitle("LogListUIClient");
	PanelUIManager::GetInstance()->AddPanel(new LogListUI);
}

PanelLoader::~PanelLoader()
{
}