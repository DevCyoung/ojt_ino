#include "pch.h"
#include "ChannelRoomMoveUI.h"
#include "InnoOJTServer.h"

#include "TrainingUI.h"
#include "InnoOJTServer.h"
#include <imgui_internal.h>

#define gLogListUI (static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUI")))

ChannelRoomMoveUI::ChannelRoomMoveUI()
{
	SetTitle("ChannelRoomMoveUI");
}

ChannelRoomMoveUI::~ChannelRoomMoveUI()
{
}

void ChannelRoomMoveUI::drawForm()
{
	ImGuiWindowClass window_GraphUI1;
	window_GraphUI1.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_GraphUI1);
	ImGui::Begin("ChannelRoomMoveUI");
	//ImGui::Text("hello ChannelRoomMoveUI");

	InnoOJTServer* server = InnoOJTServer::GetInstance();

	static int item = 0;
	
	ImGui::PushItemWidth(100);
	ImGui::InputInt("##id", &item, 0);
	ImGui::PopItemWidth();

	ImGui::SameLine();

	if (ImGui::Button("Enter Training Room"))
	{
		server->EnterRoom(item);
	}

	ImGui::SameLine();

	if (ImGui::Button("Exit Training Room"))
	{
		server->ExitRoom(item);
	}

	ImGui::SameLine();
	//BroadCast
	const tInnoRoom& room = InnoOJTServer::GetInstance()->mRoom;
	if (false == room.bTraining) // »∆∑√Ω√¿€
	{

		if (ImGui::Button("Training Start"))
		{
			if (room.clients.size() < 2)
			{
				gLogListUI->WriteWarning("Room1: User count not 2");
			}
			else
			{
				gLogListUI->WriteLine("Room1: Training Start");

				for (int i = 0; i < room.clients.size(); ++i)
				{
					InnoOJTServer::GetInstance()->SendLog(room.clients[i].ClientID, strlen("Training Start"), "Training Start");				
					InnoOJTServer::GetInstance()->SendStart(room.clients[i].ClientID);
				}

				for (int i = 0; i < room.clients.size(); ++i)
				{
					for (int j = 0; j < room.clients.size(); ++j)
					{
						if (i == j)
						{
							continue;
						}

						char otherCarBuff[256] = { 0, };						
						sprintf_s(otherCarBuff, "Other Car : %s", room.clients[i].Name.c_str());
						InnoOJTServer::GetInstance()->SendLog(room.clients[j].ClientID, strlen(otherCarBuff), otherCarBuff);
					}
				}

				InnoOJTServer::GetInstance()->mRoom.bTraining = true;
			}
		}
	}
	else // »∆∑√¡æ∑·
	{
		if (ImGui::Button("Training End"))
		{
			for (int i = 0; i < room.clients.size(); ++i)
			{
				char trainingBuff[256] = { 0, };
				sprintf_s(trainingBuff, "client id : %d End", room.clients[i].ClientID);

				gLogListUI->WriteLine(trainingBuff);
				InnoOJTServer::GetInstance()->SendStop(room.clients[i].ClientID);
			}

			InnoOJTServer::GetInstance()->RoomInit();
		}
	}
	ImGui::End();



	//ImGui::Begin("TrainingUI");

	//SelectRoom



	//ImGui::End();
}
