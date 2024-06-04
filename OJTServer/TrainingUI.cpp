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
	ImGui::Text("Hello TrainingUI");
	ImGui::End();
}
