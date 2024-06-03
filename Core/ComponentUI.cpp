# include "pch.h"
#include "Component.h"


void ComponentUI(Component* component)
{
	
	ImGui::Text("%d", component->GetID());
}
	