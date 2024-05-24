#include "pch.h"
#include "ItemUI.h"
#include "LogUI.h"
#include "PanelUIManager.h"
//#include "InspectorUI.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "ComputeShader.h"
#include "Resource.h"

ItemUI::ItemUI()
	:mResource(nullptr)
{
}

ItemUI::~ItemUI()
{
	mem::del::DeleteVectorElements(&mChildItemUIs);
}

ItemUI* ItemUI::FindItemUIOrNull(const std::string& itemUIName)
{
	for (ItemUI* itemUI : mChildItemUIs)
	{
		if (itemUI->GetName() == itemUIName)
		{
			return itemUI;
		}
	}
	return nullptr;
}

void ItemUI::render()
{
	ImGui::Bullet();
	ImGui::Selectable(mItemName.c_str(), false);
	//if (ImGui::IsItemClicked())
	//{
	//	//ImGui::SetMouseCursor(0);
	//	//LogUI::Log("click resource %s\n", mItemName.c_str());
	//
	//	ImGui::SetMouseCursor(0);
	//	PanelUI* inspectorUI =
	//		PanelUIManager::GetInstance()->FindPanelUIOrNull("InspectorUI");
	//
	//	std::wstring path = StringHelper::StrToWStr(mFullName);
	//	static_cast<InspectorUI*>(inspectorUI)->Register(mResource);
	//}
}
