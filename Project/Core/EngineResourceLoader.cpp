#include "pch.h"
#include "EngineResourceLoader.h"
#include "ResourceManager.h"
#include "Engine.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "ComputeShader.h"
#include "StructVertex.h"
#include "EngineMath.h"
//#include "BuildSetting.h"

#define USE2D 0

void EngineResourceLoader::loadResource(int renderTargetWIdth, int renderTargetHeight, int shadowMapWIdth, int shadowMapHeight)
{
	loadMesh();
	loadShader();

	loadComputeShader();
	loadTexture(renderTargetWIdth, renderTargetHeight, shadowMapWIdth, shadowMapHeight);
	loadMaterial();
}

void EngineResourceLoader::loadMaterial()
{
}

void EngineResourceLoader::loadMesh()
{
#pragma region 2DMesh

	///////
	//2D///
	//////
	//fill rect mesh
	{
		constexpr UINT VERTEX_COUNT = 4;
		tVertex vertexArray[VERTEX_COUNT] = {};
		//0---1
		//|   |
		//3---2
		vertexArray[0].Position = Vector3(-0.5f, 0.5f, 0.0f);
		vertexArray[0].Color = Vector4(1.0f, 0.4f, 1.0f, 1.0f);
		vertexArray[0].UV = Vector2(0.0f, 0.0f);

		vertexArray[1].Position = Vector3(0.5f, 0.5f, 0.0f);
		vertexArray[1].Color = Vector4(0.0f, 0.3f, 1.0f, 1.0f);
		vertexArray[1].UV = Vector2(1.0f, 0.0f);

		vertexArray[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
		vertexArray[2].Color = Vector4(0.0f, 0.0f, 0.4f, 1.0f);
		vertexArray[2].UV = Vector2(1.0f, 1.0f);

		vertexArray[3].Position = Vector3(-0.5f, -0.5f, 0.0f);
		vertexArray[3].Color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		vertexArray[3].UV = Vector2(0.0f, 1.0f);

		std::vector<UINT> indexes;
		indexes.reserve(10);

		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		gResourceManager->Insert(L"FillRect2D",
			new Mesh(vertexArray, VERTEX_COUNT, sizeof(tVertex),
				indexes.data(), indexes.size(), sizeof(UINT)));
	}

	//line rect mesh
	{
		constexpr UINT VERTEX_COUNT = 4;
		tVertex vertexArray[VERTEX_COUNT] = {};
		//0---1
		//|   |
		//3---2
		vertexArray[0].Position = Vector3(-0.5f, 0.5f, 0.0f);
		vertexArray[0].Color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexArray[0].UV = Vector2(0.0f, 0.0f);

		vertexArray[1].Position = Vector3(0.5f, 0.5f, 0.0f);
		vertexArray[1].Color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexArray[1].UV = Vector2(1.0f, 0.0f);

		vertexArray[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
		vertexArray[2].Color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexArray[2].UV = Vector2(1.0f, 1.0f);

		vertexArray[3].Position = Vector3(-0.5f, -0.5f, 0.0f);
		vertexArray[3].Color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexArray[3].UV = Vector2(0.0f, 1.0f);

		std::vector<UINT> indexes;
		indexes.reserve(10);

		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);
		indexes.push_back(3);
		indexes.push_back(0);


		gResourceManager->Insert(L"LineRect2D",
			new Mesh(vertexArray, VERTEX_COUNT, sizeof(tVertex),
				indexes.data(), indexes.size(), sizeof(UINT)));
	}

	//fill circle mesh
	{
		constexpr UINT VERTEX_COUNT = 100;
		tVertex vertexArray[VERTEX_COUNT] = {};
		//0---1
		//|   |
		//3---2

		// pivot
		vertexArray[0].Position = Vector3(0.0f, 0.0f, 0.0f);
		vertexArray[0].Color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexArray[0].UV = Vector2(0.0f, 0.0f);

		const float anglePer = 360.f / (VERTEX_COUNT - 1);

		for (int i = 1; i < VERTEX_COUNT; ++i)
		{
			const float y = sin(Deg2Rad(anglePer * (i - 1))) * 0.5f;
			const float x = cos(Deg2Rad(anglePer * (i - 1))) * 0.5f;

			const float uvx = x * 0.5f;
			const float uvy = -y * 0.5f;


			//cos(Rad2DE)

			vertexArray[i].Position = Vector3(x, y, 0.0f);
			vertexArray[i].Color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
			vertexArray[i].UV = Vector2(uvx, uvy);
		}

		std::vector<UINT> indexes;
		indexes.reserve(VERTEX_COUNT);

		//pivot
		for (int i = 0; i < VERTEX_COUNT - 2; ++i)
		{
			indexes.push_back(0);
			indexes.push_back(1 + i);
			indexes.push_back(2 + i);
		}

		indexes.push_back(1);
		indexes.push_back(0);
		indexes.push_back(VERTEX_COUNT - 1);




		gResourceManager->Insert(L"FillCircle2D",
			new Mesh(vertexArray, VERTEX_COUNT, sizeof(tVertex),
				indexes.data(), indexes.size(), sizeof(UINT)));
	}

	//line circle mesh
	{
		constexpr UINT VERTEX_COUNT = 100;
		tVertex vertexArray[VERTEX_COUNT] = {};
		//0---1
		//|   |
		//3---2

		// pivot
		vertexArray[0].Position = Vector3(0.0f, 0.0f, 0.0f);
		vertexArray[0].Color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexArray[0].UV = Vector2(0.0f, 0.0f);

		const float anglePer = 360.f / (VERTEX_COUNT);

		for (int i = 0; i < VERTEX_COUNT; ++i)
		{
			const float x = cos(Deg2Rad(anglePer * (i - 1))) * 0.5f;
			const float y = sin(Deg2Rad(anglePer * (i - 1))) * 0.5f;

			const float uvx = x + 0.5f;
			const float uvy = -y + 0.5f;

			vertexArray[i].Position = Vector3(x, y, 0.0f);
			vertexArray[i].Color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
			vertexArray[i].UV = Vector2(uvx, uvy);
		}

		std::vector<UINT> indexes;
		indexes.reserve(VERTEX_COUNT);

		//pivot
		for (int i = 0; i < VERTEX_COUNT; ++i)
		{
			indexes.push_back(i);
		}
		indexes.push_back(0);

		gResourceManager->Insert(L"LineCircle2D",
			new Mesh(vertexArray, VERTEX_COUNT, sizeof(tVertex),
				indexes.data(), indexes.size(), sizeof(UINT)));
	}

	//line mesh
	{
		constexpr UINT VERTEX_COUNT = 2;
		tVertex vertexArray[VERTEX_COUNT] = {};

		vertexArray[0].Position = Vector3(-0.5f, 0.0f, 0.0f);
		vertexArray[0].Color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexArray[0].UV = Vector2(0.0f, 0.0f);

		vertexArray[1].Position = Vector3(0.5f, 0.0f, 0.0f);
		vertexArray[1].Color = Vector4(1.0f, 0.0f, 1.0f, 1.0f);
		vertexArray[1].UV = Vector2(1.0f, 1.0f);

		std::vector<UINT> indexes;
		indexes.reserve(10);

		indexes.push_back(0);
		indexes.push_back(1);

		gResourceManager->Insert(L"Line",
			new Mesh(vertexArray, VERTEX_COUNT, sizeof(tVertex),
				indexes.data(), indexes.size(), sizeof(UINT)));
	}
#pragma endregion
#pragma region Rect
	///////
	//3D///
	//////
	{
		// =============
		// RectMesh ����
		// =============
		// 0 --- 1 
		// |  \  |
		// 3 --- 2
		constexpr UINT VERTEX_COUNT = 4;
		tVertex vertexArray[VERTEX_COUNT] = {};

		vertexArray[0].Position = Vector3(-0.5f, 0.5f, 0.f);
		vertexArray[0].Color = Vector4(1.f, 0.f, 0.f, 1.f);
		vertexArray[0].UV = Vector2(0.f, 0.f);

		vertexArray[0].Normal = Vector3(0.f, 0.f, -1.f);
		vertexArray[0].Tangent = Vector3(1.f, 0.f, 0.f);
		vertexArray[0].Binormal = Vector3(0.f, -1.f, 0.f);
		//vecVtx.push_back(v);

		vertexArray[1].Position = Vector3(0.5f, 0.5f, 0.f);
		vertexArray[1].Color = Vector4(0.f, 1.f, 0.f, 1.f);
		vertexArray[1].UV = Vector2(1.f, 0.f);

		vertexArray[1].Normal = Vector3(0.f, 0.f, -1.f);
		vertexArray[1].Tangent = Vector3(1.f, 0.f, 0.f);
		vertexArray[1].Binormal = Vector3(0.f, -1.f, 0.f);
		//vecVtx.push_back(v);

		vertexArray[2].Position = Vector3(0.5f, -0.5f, 0.f);
		vertexArray[2].Color = Vector4(0.f, 0.f, 1.f, 1.f);
		vertexArray[2].UV = Vector2(1.f, 1.f);

		vertexArray[2].Normal = Vector3(0.f, 0.f, -1.f);
		vertexArray[2].Tangent = Vector3(1.f, 0.f, 0.f);
		vertexArray[2].Binormal = Vector3(0.f, -1.f, 0.f);
		//vecVtx.push_back(v);

		vertexArray[3].Position = Vector3(-0.5f, -0.5f, 0.f);
		vertexArray[3].Color = Vector4(0.f, 0.f, 0.f, 1.f);
		vertexArray[3].UV = Vector2(0.f, 1.f);

		vertexArray[3].Normal = Vector3(0.f, 0.f, -1.f);
		vertexArray[3].Tangent = Vector3(1.f, 0.f, 0.f);
		vertexArray[3].Binormal = Vector3(0.f, -1.f, 0.f);
		//vecVtx.push_back(v);

		std::vector<UINT> indexes;
		indexes.reserve(10);

		indexes.push_back(0);
		indexes.push_back(2);
		indexes.push_back(3);

		indexes.push_back(0);
		indexes.push_back(1);
		indexes.push_back(2);

		gResourceManager->Insert(L"Panel",
			new Mesh(vertexArray, VERTEX_COUNT, sizeof(tVertex),
				indexes.data(), indexes.size(), sizeof(UINT)));
	}
#pragma endregion
}

void EngineResourceLoader::loadTexture(int renderTargetWIdth, int renderTargetHeight, int shadowMapWIdth, int shadwMapHeight)
{
	{
		Texture* const defaultTexture = new Texture(1024, 1024,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE,
			D3D11_USAGE::D3D11_USAGE_DEFAULT);

		gResourceManager->Insert(L"TextureCS", defaultTexture);
	}

	{
		XMUINT2 size = { static_cast<UINT>(renderTargetWIdth),
						 static_cast<UINT>(renderTargetHeight) };

		Texture* const copyRenderTargetTexture = new Texture(size.x, size.y,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE,
			D3D11_USAGE::D3D11_USAGE_DEFAULT);

		gResourceManager->Insert(L"CopyRenderTargetTexture", copyRenderTargetTexture);
	}

	{
		XMUINT2 size = { static_cast<UINT>(200),
						 static_cast<UINT>(200) };

		Texture* const dummyUI = new Texture(size.x, size.y,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE,
			D3D11_USAGE::D3D11_USAGE_DEFAULT);

		gResourceManager->Insert(L"DummyUI", dummyUI);
	}
}

void EngineResourceLoader::loadShader()
{

#ifdef  USE2D
#pragma region 2D SHader
	//Fill2D Debug
	{
		Shader* const debugShader =
			new Shader(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
				L"\\Shader\\VSFillDebug2D.hlsl", L"main",
				L"\\Shader\\PSFillDebug2D.hlsl", L"main",
				eSMType::Default,
				eRSType::CullNone,
				eDSType::None,
				eBSType::AlphaBlend);
		gResourceManager->Insert(L"FillDebug2D", debugShader);
	}

	//Grid2D Debug
	{
		Shader* const debugGridShader =
			new Shader(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
				L"\\Shader\\VSGrid2D.hlsl", L"main",
				L"\\Shader\\PSGrid2D.hlsl", L"main",
				eSMType::Default,
				eRSType::CullNone,
				eDSType::None,
				eBSType::AlphaBlend);
		gResourceManager->Insert(L"DebugGrid2D", debugGridShader);
	}

	//Line2D Debug
	{
		Shader* const debugShader =
			new Shader(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
				L"\\Shader\\VSLineDebug2D.hlsl", L"main",
				L"\\Shader\\PSLineDebug2D.hlsl", L"main",
				eSMType::Default,
				eRSType::CullNone,
				eDSType::None,
				eBSType::AlphaBlend);
		gResourceManager->Insert(L"LineDebug2D", debugShader);
	}

	//Sprite 
	{
		Shader* const sprite2DShader =
			new Shader(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
				L"\\Shader\\VSSprite2D.hlsl", L"main",
				L"\\Shader\\PSSprite2D.hlsl", L"main",
				eSMType::Default,
				eRSType::CullNone,
				eDSType::LessEqual,
				eBSType::AlphaBlend);
		gResourceManager->Insert(L"Sprite2D", sprite2DShader);
	}

	//UI
	{
		Shader* const UIShader =
			new Shader(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
				L"\\Shader\\VSUI2D.hlsl", L"main",
				L"\\Shader\\PSUI2D.hlsl", L"main",
				eSMType::Default,
				eRSType::CullNone,
				eDSType::None,
				eBSType::AlphaBlend);
		gResourceManager->Insert(L"UI2D", UIShader);
	}

	//Light
	//Light Sprite
	{
		Shader* const sprite2DShader =
			new Shader(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
				L"\\Shader\\VSLightSprite2D.hlsl", L"main",
				L"\\Shader\\PSLightSprite2D.hlsl", L"main",
				eSMType::Default,
				eRSType::CullNone,
				eDSType::LessEqual,
				eBSType::AlphaBlend);
		gResourceManager->Insert(L"LightSprite2D", sprite2DShader);
	}
#pragma endregion
#endif
}

void EngineResourceLoader::loadComputeShader()
{
	//stdCS
	{
		ComputeShader* const stdCS =
			new ComputeShader(L"\\Shader\\stdCS.hlsl", L"main");

		gResourceManager->Insert(L"StdCS", stdCS);
	}
}
