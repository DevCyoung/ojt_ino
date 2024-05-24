#include "pch.h"
#include "EditorViewUI.h"
#include "SceneManager.h"
#include "Engine.h"
#include "Texture.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameSystem.h"
#include "RenderTargetRenderer.h"
#include "GraphicDeviceDx11.h"
#include "WindowManager.h"
#include "AlphaHelper.h"

#include "TimeManager.h"
#include "InputManager.h"
#include "Transform.h"
#include "GameObject.h"
#include "Camera.h"
#include "CameraInputMoveMent.h"
#include "RenderTargetRenderer.h"
#include "Builder.h"
//#include

//#include <ImGUI/imgui_filter.h>

EditorViewUI::EditorViewUI()
	: mEditorCamera(nullptr)
{
	SetTitle("EditorViewUI");
	//Editor Camera
	{
		const Vector2 screenSize = Vector2(1280, 720);
		GameObject* const mainCamera = CreateGameObject();
		mainCamera->AddComponent<Camera>();
		mainCamera->AddComponent<CameraInputMoveMent>();

		mainCamera->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
		mainCamera->GetComponent<Camera>()->SetRenderTargetSize(screenSize);
		mainCamera->GetComponent<Camera>()->SetPriorityType(eCameraPriorityType::Editor);
		mainCamera->GetComponent<Camera>()->SetProjectionType(eCameraProjectionType::Perspective);
		mainCamera->GetComponent<Camera>()->TurnOnAllLayer();		
		mEditorCamera = mainCamera;		
	}
}

EditorViewUI::~EditorViewUI()
{
	DELETE_POINTER(mEditorCamera);
}

void EditorViewUI::update()
{
}

void EditorViewUI::drawForm()
{
	//EditorView 전용텍스처에 그린다.
	Camera* editorCamera = mEditorCamera->GetComponent<Camera>();
	RenderTargetRenderer* renderer = gCurrentSceneRenderer;
	Texture* renderTex = gResourceManager->Find<Texture>(L"/Editor/EditorViewRenderTexture");
	Texture* depThTex = gResourceManager->Find<Texture>(L"/Editor/EditorViewDepthTexture");
	Scene* currentScene = gCurrentScene;
	FLOAT backgroundColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };
	UINT cameraMask = renderer->GetCameraLayerMask();

	renderer->RegisterRenderCamera(editorCamera);
	renderer->TurnOffAllCamera();
	renderer->TurnOnCamera(eCameraPriorityType::Editor);
	mEditorCamera->GetComponent<Transform>()->CalculateTransform();
	editorCamera->CalculateCamera();

	gGraphicDevice->ClearRenderTarget(
		renderTex->GetAddressOf(),
		depThTex->GetDSV(), backgroundColor);
	currentScene->Render(
		static_cast<UINT>(renderTex->GetWidth()),
		static_cast<UINT>(renderTex->GetHeight()),
		renderTex->GetAddressOf(),
		depThTex->GetDSV());

	renderer->SetCameraLayerMask(cameraMask);
	renderer->PopUpCamera(eCameraPriorityType::Editor);	
	renderer->TurnOffCamera(eCameraPriorityType::Editor);

	//둘다 UAV여야함	ImGUI 용도로 사용하기위해 Alpha값 1로 덮어쓴다.
	Texture* rwTex = gResourceManager->Find<Texture>(L"/Editor/EditorViewRWTexture");
	Texture* rwTex2 = gResourceManager->Find<Texture>(L"/Editor/EditorViewCopyRWTexture");
	gGraphicDevice->CopyResource(rwTex2->GetID3D11Texture2D(), renderTex->GetID3D11Texture2D());
	TextureAlphaTo(rwTex, rwTex2);
	gGraphicDevice->CopyResource(renderTex->GetID3D11Texture2D(), rwTex->GetID3D11Texture2D());
	Engine::GetInstance()->OmSet();

	ImGui::Begin("Editor View");
	ID3D11ShaderResourceView* texture = renderTex->GetSRV();
	ImVec2 cursurScreenPos = ImGui::GetCursorScreenPos();
	ImVec2 renderTargetSize = ImVec2(renderTex->GetWidth(), renderTex->GetHeight());

	if (ImGui::IsWindowFocused())
	{
		mEditorCamera->GetComponent<CameraInputMoveMent>()->MoveCamera();
	}

	POINT ptMousePos = {};
	GetCursorPos(&ptMousePos);
	ImVec2 mousePos = 
		ImVec2(static_cast<float>(ptMousePos.x), static_cast<float>(ptMousePos.y));
	ImGui::Image((void*)texture, renderTargetSize);
	ImVec2 screenMousePos = ImVec2(mousePos.x - cursurScreenPos.x,
		mousePos.y - cursurScreenPos.y);

	ImGui::Text("cursur posX%f cursur  posY%f", cursurScreenPos.x, cursurScreenPos.y);
	ImGui::Text("mouse posX%d mouse posY%d", ptMousePos.x, ptMousePos.y);
	ImGui::Text("In Screen Mouse Pos X%f Y%f", screenMousePos.x, screenMousePos.y);
	WindowManager::GetInstance()->SetWindowScreenPos(Vector2(screenMousePos.x, screenMousePos.y));


	Transform* transform = editorCamera->GetComponent<Transform>();
	Vector3 position = transform->GetPosition();
	Vector3 scale	= transform->GetScale();
	Vector3 rotation = transform->GetRotation();

	ImGui::Text("Camera pos X%f Y%f Z%f", position.x, position.y, position.z);
	ImGui::Text("Camera scale X%f Y%f Z%f", scale.x, scale.y, scale.z);
	ImGui::Text("Camera rotation X%f Y%f Z%f", rotation.x, rotation.y, rotation.z);		

	ImGui::End();
}
