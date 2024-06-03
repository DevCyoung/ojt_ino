# include "pch.h"
#include "MaterialUI.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Mesh.h"

class RenderComponent;
void RenderComponentUI(RenderComponent* component);

void MeshRendererUI(MeshRenderer* component)
{
	RenderComponentUI(component);

	ImGui::Separator();

	//Material
	for (UINT i = 0; i < component->GetMaterialCount(); ++i)
	{
		Material* material = component->GetMaterial(i);
		std::string name = StringHelper::WStrToStr(material->GetRelativePathorName());

		if (name.length() <= 1)
		{
			name = "NONE";
		}

		name += "##";
		name += std::to_string(component->GetID());
		name += "_";
		name += std::to_string(i);

		if (ImGui::CollapsingHeader(name.c_str()))
		{
			MaterialUI(component->GetMaterial(i));
		}		
	}
	ImGui::Separator();
}
	