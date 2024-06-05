#include "pch.h"
#include "TrainingUI.h"
#include "InnoOJTServer.h"

TrainingUI::TrainingUI()
{
	SetTitle("TrainingUI");
}

TrainingUI::~TrainingUI()
{
}

void TrainingUI::drawForm()
{
	ImGui::Begin("TrainingUI");	

	const tInnoRoom& room = InnoOJTServer::GetInstance()->mRoom;

	if (ImGui::Button("Traing Start"))
	{
		

		for (int i = 0; i < room.clients.size(); ++i)
		{			
			InnoOJTServer::GetInstance()->SendStart(room.clients[i].ClientID);
		}

		InnoOJTServer::GetInstance()->mRoom.bTraining = true;
		//InnoOJTServer::GetInstance()->SendLog(0, strlen("hello client"), "hello client");		
	}
	ImGui::SameLine();
	if (ImGui::Button("Traing End"))
	{
		for (int i = 0; i < room.clients.size(); ++i)
		{
			InnoOJTServer::GetInstance()->SendStop(room.clients[i].ClientID);
		}		
	}
	ImGui::End();
}
	