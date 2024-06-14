#include "pch.h"
#include "ItemUI.h"
#include "PanelUIManager.h"
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
}
