#include "pch.h"
#include "InnoInputUI.h"
#include <RenderTargetRenderer.h>
#include <GameSystem.h>
#include <SceneManager.h>
#include <AlphaHelper.h>

InnoInputUI::InnoInputUI()
	: mEditorCamera(nullptr)
{

	const int width = 800;
	const int height = 400;

	//Editor Camera
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
		mainCamera->GetComponent<Camera>()->Set2DSize(1.f);
		mEditorCamera = mainCamera;
	}
}

InnoInputUI::~InnoInputUI()	
{
}


#define offset 16.f

void InnoInputUI::drawForm()
{




#pragma region InputScreen

	//EditorView 전용텍스처에 그린다.
	Camera* editorCamera = mEditorCamera->GetComponent<Camera>();
	RenderTargetRenderer* renderer = gCurrentSceneRenderer;
	Texture* renderTex = gResourceManager->Find<Texture>(L"/Editor/EditorViewRenderTexture");
	Texture* depThTex = gResourceManager->Find<Texture>(L"/Editor/EditorViewDepthTexture");
	Scene* currentScene = gCurrentScene;
	FLOAT backgroundColor[4] = {0.5f, 0.5f, 0.5f, 1.f };
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

	ImGui::Begin("ScreenUI");

#pragma region Screen
	//Texture* rwTex = gResourceManager->Find<Texture>(L"/Editor/EditorViewRenderTexture");
	ImVec2 renderTargetSize = ImVec2(rwTex->GetWidth(), rwTex->GetHeight());
	//Texture* depThTex = gResourceManager->Find<Texture>(L"/Editor/EditorViewDepthTexture");
	//FLOAT backgroundColor[4] = {1.f, 0.0f, 1.f, 1.f };

	//gGraphicDevice->ClearRenderTarget(
	//	rwTex->GetAddressOf(),
	//depThTex->GetDSV(), backgroundColor);

	if (ImGui::IsWindowFocused())
	{
		mEditorCamera->GetComponent<CameraInputMoveMent>()->MoveCamera();
	}
	ImGui::Image((void*)renderTex->GetSRV(), renderTargetSize);
#pragma endregion

	static float testFloat = 0.f;


	ImGui::Separator();	
	ImGui::PushItemWidth(800.f);
	ImGui::SliderFloat("##testFloat", &testFloat, 0.f, 100000.f);
	ImGui::PopItemWidth();
	ImGui::Separator();

	ImGui::Button("Play");

	ImGui::SameLine();

	ImGui::Button(">>");

	ImGui::SameLine();

	ImGui::Button("<<");

	ImGui::SameLine( 600.f );

	ImGui::Text("Play Time : 00.00 / 00.00 ");

	//ImGui::Separator();

	ImGui::End();

#pragma endregion InputScreen

#pragma region InputUI1
	ImGui::Begin("InputUI1");
	static char buff[256] = {};
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

	ImGui::Text("MS");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);	
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##MS", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();
	
	ImGui::Text("KS");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##KS", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("ZU");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##ZU", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("MU");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##MU", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("CS");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##CS", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("ZS");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##ZS", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();
	

	ImGui::End();
#pragma endregion InputUI1

#pragma region InputUI2
	ImGui::Begin("InputUI2");

	ImGui::Text("Speed");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##Speed", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("BumpStart");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##BumpStart", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("BumpEnd");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##BumpEnd", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("BumpAmp");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##BumpAmp", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("SamplingTime");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##SamplingTime", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::End();

#pragma endregion InputUI2

#pragma region InputUI3
	ImGui::Begin("InputUI3");

	ImGui::Text("ServerIP");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##ServerIP", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("Port");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputText("##Port", buff, 16);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	//ImGui::Text("SamplingTime");
	//ImGui::SameLine(45.0f);
	//ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	//ImGui::PushItemWidth(100.0f);
	//ImGui::InputText("##MU", buff, 16);
	//ImGui::PopItemWidth();
	//ImGui::Spacing();
	ImGui::Button("Connect");

	ImGui::End();
#pragma endregion InputUI3

#pragma region GraphUI1


	ImGui::Begin("GraphUI1");

	//ImGuiStyle style = ImGuiTheme::ThemeToStyle(ImGuiTheme::ImGuiTheme_MaterialFlat);
	//ImGuiStyle& ori = ImGui::GetStyle();
	//ori = style;

	//ThemeInfo gThemeInfos[] = {
	//ImGuiTheme::gThemeInfos[0];
	//ImGui::select

	std::vector<std::string> vec;


	static int current_item = 7;
	for (int i = ImGuiTheme::ImGuiTheme_ImGuiColorsClassic; i < ImGuiTheme::ImGuiTheme_Count; ++i)
	{
		vec.push_back(ImGuiTheme::ImGuiTheme_Name((ImGuiTheme::ImGuiTheme_)i));
	}

	if (ImGui::Combo("combo", &current_item, vec))
	{
		
	}

	{
		ImGuiStyle newStyle = ImGuiTheme::ThemeToStyle(ImGuiTheme::ImGuiTheme_(current_item));
		ImGuiStyle& style = ImGui::GetStyle();
		style = newStyle;
	}

	ImGui::End();
#pragma endregion GraphUI1

}
