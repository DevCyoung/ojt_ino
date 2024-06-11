#include "pch.h"
#include "InnoInputUI.h"
#include <RenderTargetRenderer.h>
#include <GameSystem.h>
#include <SceneManager.h>
#include <AlphaHelper.h>
#include <Texture.h>

InnoInputUI::InnoInputUI()
	: mSceneRenderHelperA(nullptr)
	, mSceneRenderHelperB(nullptr)
{
	mSceneRenderHelperA = new SceneRenderHelper(L"PlayerA", 800, 200);
	mSceneRenderHelperB = new SceneRenderHelper(L"PlayerB", 800, 200);
}

InnoInputUI::~InnoInputUI()	
{
	DELETE_POINTER(mSceneRenderHelperA);
	DELETE_POINTER(mSceneRenderHelperB);
}


#define offset 16.f

void InnoInputUI::drawForm()
{
#pragma region InputScreen
	ImGui::Begin("ScreenUI");
	mSceneRenderHelperA->Draw(gCurrentScene);
	mSceneRenderHelperB->Draw(gCurrentScene);

	Texture* renderTexA = mSceneRenderHelperA->GetRenderTexture();
	Texture* renderTexB = mSceneRenderHelperB->GetRenderTexture();

	ImVec2 renderTargetSizeA = ImVec2(renderTexA->GetWidth(), renderTexA->GetHeight());
	ImVec2 renderTargetSizeB = ImVec2(renderTexB->GetWidth(), renderTexB->GetHeight());

#pragma region Screen	
	if (ImGui::IsWindowFocused())
	{
		mSceneRenderHelperA->GetCamera()->GetComponent<CameraInputMoveMent>()->MoveCamera();
	}

	ImGui::Image((void*)renderTexA->GetSRV(), renderTargetSizeA);
	ImGui::Image((void*)renderTexB->GetSRV(), renderTargetSizeB);

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
