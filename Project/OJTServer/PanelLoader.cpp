#include "pch.h"
#include "PanelLoader.h"
#include "ChannelRoomMoveUI.h"
#include "ChannelUI.h"
#include "ListenUI.h"
#include "RoomUI.h"
#include "TrainingUI.h"
#include <LogListUI.h>
#include <PanelUIManager.h>

PanelLoader::PanelLoader()
{	
	PanelUIManager::GetInstance()->AddPanel(new ChannelRoomMoveUI);
	PanelUIManager::GetInstance()->AddPanel(new ChannelUI);
	PanelUIManager::GetInstance()->AddPanel(new ListenUI);
	PanelUIManager::GetInstance()->AddPanel(new LogListUI);
	PanelUIManager::GetInstance()->AddPanel(new RoomUI);
	PanelUIManager::GetInstance()->AddPanel(new TrainingUI);
}

PanelLoader::~PanelLoader()
{	
}