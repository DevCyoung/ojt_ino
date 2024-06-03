#include "pch.h"
#include "MaterialUI.h"
#include "Texture.h"
#include "Material.h"
#include "ResourceManager.h"

//#include <ImGUI/imgui_filter.h>

void MaterialUI(Material* material)
{
	const ResourceManager::Dictionary& dictionary =
		ResourceManager::GetInstance()->GetDictionary(eResourceType::Texture);

	std::vector<std::string> selectItems;

	ResourceManager::Dictionary::const_iterator citer = dictionary.cbegin();
	ResourceManager::Dictionary::const_iterator cend = dictionary.cend();

	ImVec2 renderTargetSize = ImVec2(200.f, 200.f);

	static int selectIdx = -1;
	static Texture* prevTexOrNull = nullptr;

	for (; citer != cend; ++citer)
	{
		std::string patah = StringHelper::WStrToStr(citer->first);
		if (patah.front() != '\\')
		{
			continue;
		}
		selectItems.push_back(patah);
	}

	ID3D11ShaderResourceView* dummy 
		= gResourceManager->Find<Texture>(L"\\Texture\\Dummy\\UIdummy.png")->GetSRV();

	std::string pathOrName = StringHelper::WStrToStr(material->GetRelativePathorName());
	ImGui::Text(pathOrName.c_str());

	for (int i = 0; i <= TEX_7; ++i)
	{
		ID3D11ShaderResourceView* rv = dummy;
		if (material->GetTextureOrNull((TEX_PARAM)i))
		{
			rv = material->GetTextureOrNull((TEX_PARAM)i)->GetSRV();	
		}
		std::string texNum = "Tex_";
		texNum += std::to_string(i);
		ImGui::Text(texNum.c_str());
		ImGui::Image((void*)rv, renderTargetSize);
		std::string textureSelectButton = "##ShaderSelectBtn_";
		textureSelectButton += std::to_string(i);
		ImGui::SameLine();

		if (ImGui::Button(textureSelectButton.c_str(), ImVec2(18, 18)))
		{
			selectIdx = i;
			prevTexOrNull = material->GetTextureOrNull(TEX_PARAM(i));
			ImGui::OpenPopup("Stacked 111");
		}

	}

	bool bChange = false;
	if (ImGui::BeginPopupModal("Stacked 111", NULL, ImGuiWindowFlags_MenuBar))
	{		
		if (ImGui::Button("revert##Material"))
		{
			material->SetTexture(TEX_PARAM(selectIdx), prevTexOrNull);			
		}
		ImGui::SameLine();
		if (ImGui::Button("close##Material"))
		{
			ImGui::CloseCurrentPopup();
			bChange = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("remove##Material"))
		{
			prevTexOrNull = nullptr;
			material->SetTexture(TEX_PARAM(selectIdx), nullptr);			
		}

		int texOrder = 0;
		for (const std::string& item : selectItems)
		{
			Texture* tex =
				gResourceManager->FindOrNull<Texture>(StringHelper::StrToWStr(item).c_str());

			if (ImGui::ImageButton(tex->GetSRVOrNull(), ImVec2(100.f, 100.f)))
			{
				material->SetTexture(TEX_PARAM(selectIdx), tex);				
			}

			++texOrder;


			if (texOrder % 5 != 0)
			{
				ImGui::SameLine();
			}

		}		
		ImGui::EndPopup();
	}


	if (bChange)
	{
		material->Save(material->GetRelativePathorName());
	}
}
