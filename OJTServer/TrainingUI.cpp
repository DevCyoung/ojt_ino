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
	if (ImGui::Button("Traing Start"))
	{
		const tInnoRoom& room = InnoOJTServer::GetInstance()->mRoom;

		for (int i = 0; i < room.clients.size(); ++i)
		{			
			InnoOJTServer::GetInstance()->SendStart(room.clients[i].ClientID);
		}

		InnoOJTServer::GetInstance()->mRoom.bTraining = true;
		//InnoOJTServer::GetInstance()->SendLog(0, strlen("hello client"), "hello client");		
	}
	ImGui::SameLine();
	ImGui::Button("Traing End");
	ImGui::End();
}
	