# include "pch.h"
#include "Light3D.h"
#include "Texture.h"
#include "ResourceManager.h"

class Component;
void ComponentUI(Component* component);


void Light3DUI(Light3D* component)
{
	ComponentUI(component);

	tLight3DInfo info = component->GetLightInfo();
	
	static float color[4] = { 1.f , 1.f, 1.f, 1.f };
	
	info.LightColor = Vector4(color);

	ImGui::DragFloat3("color", color, 0.025f, 0.f, 1.f);



	component->SetLightInfo(info);


	Texture* tex = gResourceManager->Find<Texture>(L"ShadowMap_0");
	ImVec2 renderTargetSize = ImVec2(500, 500);
	ImGui::Image((void*)tex->GetSRV(), renderTargetSize);
}
