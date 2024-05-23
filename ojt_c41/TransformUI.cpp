# include "pch.h"
#include "Transform.h"

class Component;
void ComponentUI(Component* component);


void TransformUI(Transform* component)
{
	ComponentUI(component);

	Vector3 position = component->GetPosition();
	Vector3 scale = component->GetScale();
	Vector3 rotation = component->GetRotation();
	//rotation = (rotation / XM_PI) * 180.f;

	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Position", position);

	ImGui::Text("Scale   ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Scale", scale);

	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Rotation", rotation);

	component->SetPosition(position);
	component->SetScale(scale);
	component->SetRotation(rotation);
}
