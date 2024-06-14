#include "pch.h"
#include "EditorResourceLoader.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Shader.h"

void EditorResourceLoader()
{
	//sahder
	{
		//UI
		{
			Shader* const UIShader =
				new Shader(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
					L"\\Shader\\VSAlphaTo.hlsl", L"main",
					L"\\Shader\\PSAlphaTo.hlsl", L"main",
					eSMType::Default,
					eRSType::CullNone,
					eDSType::None,
					eBSType::OneZero);
			gResourceManager->Insert(L"TextureAlphaTo", UIShader);
		}
	}
}
