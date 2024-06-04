#include "pch.h"
#include "TrainingUI.h"

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
	ImGui::Button("Traing Start");
	ImGui::SameLine();
	ImGui::Button("Traing End");
	ImGui::End();
}
	