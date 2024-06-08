#include "pch.h"
#include "EditorResourceLoader.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Shader.h"

static void loadRenderTexture(const UINT width, const UINT height, const std::wstring& relativePathOrName)
{
	{
		Texture* const texture = new Texture(width, height,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE | 
			D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET|
			D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS,
			D3D11_USAGE::D3D11_USAGE_DEFAULT);

		gResourceManager->Insert(relativePathOrName + L"RenderTexture", texture);
	}
	//R
	{
		Texture* const texture = new Texture(width, height,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,			
			D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS,
			D3D11_USAGE::D3D11_USAGE_DEFAULT);

		gResourceManager->Insert(relativePathOrName + L"RWTexture", texture);
	}

	{
		Texture* const texture = new Texture(width, height,
			DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT,
			D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL,
			D3D11_USAGE::D3D11_USAGE_DEFAULT);

		gResourceManager->Insert(relativePathOrName + L"DepthTexture", texture);
	}
}

void EditorResourceLoader()
{
	loadRenderTexture(800, 400,  L"/Editor/EditorView");
	loadRenderTexture(800, 400, L"/Editor/EditorViewCopy");

	loadRenderTexture(1280, 720, L"/Editor/GameView");
	loadRenderTexture(1280, 720, L"/Editor/GameViewCopy");
	
	//loadRenderTexture(1280, 720,  L"/Editor/EditorView");
	//loadRenderTexture(1280, 720, L"/Editor/EditorViewCopy");
	//loadRenderTexture(1280, 720, L"/Editor/GameView");
	//loadRenderTexture(1280, 720, L"/Editor/GameViewCopy");

	//loadRenderTexture(1280, 720, L"/Editor/HDCopy2");
	//loadRenderTexture(1920, 1080, L"/Editor/FHD");
	//loadRenderTexture(1920, 1080, L"/Editor/FHDCopy");
	//loadRenderTexture(800,  600,  L"/Editor/SVGAT");
	//loadRenderTexture(800, 600, L"/Editor/SVGATCopy");


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
