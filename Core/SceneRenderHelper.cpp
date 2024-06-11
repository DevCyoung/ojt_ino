#include "pch.h"
#include "SceneRenderHelper.h"
#include "Builder.h"
#include "Camera.h"
#include "CameraInputMoveMent.h"
#include "Transform.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"

SceneRenderHelper::SceneRenderHelper(const std::wstring& label, const int width, const int height)
	: mCamera(nullptr)
	, mRenderTex(nullptr)
	, mDepThTex(nullptr)
	, mRWTex(nullptr)
	, RWTex2(nullptr)
	, mLabel(label)
	, mBackgroundColor{ 0.5f, 0.5f, 0.5f, 1.f }
{
	const Vector2 screenSize = Vector2(width, height);
	GameObject* const mainCamera = CreateGameObject();
	mainCamera->AddComponent<Camera>();

	mainCamera->AddComponent<CameraInputMoveMent>();
	mainCamera->GetComponent<Transform>()->SetPosition(0.f, 0.f, -2.5f);
	mainCamera->GetComponent<Camera>()->SetRenderTargetSize(screenSize);
	mainCamera->GetComponent<Camera>()->SetPriorityType(eCameraPriorityType::Editor);
	mainCamera->GetComponent<Camera>()->SetProjectionType(eCameraProjectionType::Orthographic);
	mainCamera->GetComponent<Camera>()->TurnOnAllLayer();
	mainCamera->GetComponent<Camera>()->Set2DSize(1.2f);
	mCamera = mainCamera;
	
	//Texture
	{
		mRenderTex = new Texture(width, height,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE |
			D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET |
			D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS,
			D3D11_USAGE::D3D11_USAGE_DEFAULT);

		gResourceManager->Insert(mLabel + L":SceneRenderHelper:RenderTexture", mRenderTex);
	}
	{
		mDepThTex = new Texture(width, height,
			DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT,
			D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL,
			D3D11_USAGE::D3D11_USAGE_DEFAULT);

		gResourceManager->Insert(mLabel + L":SceneRenderHelper:DepthTexture", mDepThTex);
	}

	{
		mRWTex = new Texture(width, height,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS,
			D3D11_USAGE::D3D11_USAGE_DEFAULT);

		gResourceManager->Insert(mLabel + L":SceneRenderHelper:RWTexture", mRWTex);
	}

	{
		RWTex2 = new Texture(width, height,
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
			D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS,
			D3D11_USAGE::D3D11_USAGE_DEFAULT);

		gResourceManager->Insert(mLabel + L":SceneRenderHelper:CopyRWTexture", RWTex2);
	}
}

SceneRenderHelper::~SceneRenderHelper()
{
	DELETE_POINTER(mCamera);
}

void SceneRenderHelper::Draw(Scene* scene)
{
	//EditorView 전용텍스처에 그린다.
	Camera* camera = mCamera->GetComponent<Camera>();
	RenderTargetRenderer* renderer = gCurrentSceneRenderer;	
	UINT cameraMask = renderer->GetCameraLayerMask();

	renderer->RegisterRenderCamera(camera);
	renderer->TurnOffAllCamera();
	renderer->TurnOnCamera(eCameraPriorityType::Editor);
	mCamera->GetComponent<Transform>()->CalculateTransform();
	camera->CalculateCamera();

	gGraphicDevice->ClearRenderTarget(
		mRenderTex->GetAddressOf(),
		mDepThTex->GetDSV(), mBackgroundColor);

	scene->Render(
		static_cast<UINT>(mRenderTex->GetWidth()),
		static_cast<UINT>(mRenderTex->GetHeight()),
		mRenderTex->GetAddressOf(),
		mDepThTex->GetDSV());

	renderer->SetCameraLayerMask(cameraMask);
	renderer->PopUpCamera(eCameraPriorityType::Editor);
	renderer->TurnOffCamera(eCameraPriorityType::Editor);

	//둘다 UAV여야함	ImGUI 용도로 사용하기위해 Alpha값 1로 덮어쓴다.	
	gGraphicDevice->CopyResource(RWTex2->GetID3D11Texture2D(), mRenderTex->GetID3D11Texture2D());
	TextureAlphaTo(mRWTex, RWTex2);
	gGraphicDevice->CopyResource(mRenderTex->GetID3D11Texture2D(), mRWTex->GetID3D11Texture2D());
	Engine::GetInstance()->OmSet();
}
