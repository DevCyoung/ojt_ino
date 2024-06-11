#include "pch.h"
#include "InnoInputUI.h"
#include <RenderTargetRenderer.h>
#include <GameSystem.h>
#include <SceneManager.h>
#include <AlphaHelper.h>
#include <Texture.h>
#include "InnoSimulator.h"
#include "InnoDataManager.h"
#include <TimeManager.h>
#include <implot.h>

InnoInputUI::InnoInputUI()
	: mSceneRenderHelperA(nullptr)
	, mSceneRenderHelperB(nullptr)
{
	mSceneRenderHelperA = new SceneRenderHelper(L"PlayerA", 800, 200);
	mSceneRenderHelperB = new SceneRenderHelper(L"PlayerB", 800, 200);

	ImPlot::CreateContext();
}

InnoInputUI::~InnoInputUI()	
{
	DELETE_POINTER(mSceneRenderHelperA);
	DELETE_POINTER(mSceneRenderHelperB);

	ImPlot::DestroyContext();
}


#define offset 16.f

void InnoInputUI::drawForm()
{
	ImPlot::ShowDemoWindow();	

	InnoSimulator* innoSimulator = InnoSimulator::GetInstance();

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

	eInnoSimulatorState simulatorState = innoSimulator->GetSimulatorState();

	std::string button1Name = "Play";

	if (simulatorState == eInnoSimulatorState::Playing)
	{
		button1Name = "Playing";
	}

	if (ImGui::Button(button1Name.c_str()) && simulatorState == eInnoSimulatorState::None)
	{
		innoSimulator->Play();
	}

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

	float MS			= innoSimulator->GetMS();
	float MU			= innoSimulator->GetMU();
	float KS			= innoSimulator->GetKS();
	float CS			= innoSimulator->GetCS();
	float KT			= innoSimulator->GetKT();
	float Speed			= innoSimulator->GetSpeed();
	float BumpStart		= innoSimulator->GetBumpStart();
	float BumpEnd		= innoSimulator->GetBumpEnd();
	float BumpAmp		= innoSimulator->GetBumpAmp();
	float SamplingTime	= innoSimulator->GetSamplintTIme();	

	static char buff[256] = {};

	ImGui::Begin("InputUI1");
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

	ImGui::Text("MS");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);	
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##MS", &MS, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();
	
	ImGui::Text("MU");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##MU", &MU, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("KS");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##KS", &KS, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("CS");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##CS", &CS, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("KT");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##KT", &KT, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();	

	ImGui::End();
#pragma endregion InputUI1

#pragma region InputUI2
	ImGui::Begin("InputUI2");

	ImGui::Text("Speed23");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##Speed:InputUI2", &Speed, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("BumpStart");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##BumpStart:InputUI2", &BumpStart, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("BumpEnd");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##BumpEnd:InputUI2", &BumpEnd, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("BumpAmp");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##BumpAmp:InputUI2", &BumpAmp, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("SamplingTime");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##SamplingTime", &SamplingTime, 0.f, 0.f);
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

	struct Funcs
	{
		static float ZsPos(const std::vector<tInnoSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZsPos; }
		static float ZsSpeed(const std::vector<tInnoSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZsSpeed; }
		static float ZsAcc(const std::vector<tInnoSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZsAcc; }
		static float ZuPos(const std::vector<tInnoSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZuPos; }
		static float ZuSpeed(const std::vector<tInnoSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZuSpeed; }
		static float ZuAcc(const std::vector<tInnoSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZuAcc; }
		static float Zr(const std::vector<tInnoSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].Zr; }
		static float XPos(const std::vector<tInnoSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].xPos; }
		static float XSpeed(const std::vector<tInnoSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].xSpeed; }
		//static float Saw(void*, int i) { return (i & 1) ? 1.0f : -1.0f; }
	};

	float (*funcZsPos)      (void*, int) = (float (*)(void*, int))Funcs::ZsPos;
	float (*funcZsSpeed)    (void*, int) = (float (*)(void*, int))Funcs::ZsSpeed;
	float (*funcZsAcc)      (void*, int) = (float (*)(void*, int))Funcs::ZsAcc;
	float (*funcZuPos)      (void*, int) = (float (*)(void*, int))Funcs::ZuPos;
	float (*funcZuSpeed)    (void*, int) = (float (*)(void*, int))Funcs::ZuSpeed;
	float (*funcZuAcc)      (void*, int) = (float (*)(void*, int))Funcs::ZuAcc;
	float (*funcZr)         (void*, int) = (float (*)(void*, int))Funcs::Zr;

	const float GRAPH_MIN = -0.05f;
	const float GRAPH_MAX = 0.05f;
	const float GRAPH_HEIGHT = 100.f;

	//마지막 90개만 그리면된다.
	const std::vector<tInnoSampleData>& playerASampleDatas = InnoDataManager::GetInstance()->GetPlayerASampleDatas();

	const int maxValueSize = 800;
	static float frameTime = 0.f;

	static std::vector<float> ZSPoses(maxValueSize, 0.f); //120 프레임에 하나씩등록
	static std::vector<float> times(maxValueSize, 0.f);

	frameTime += gDeltaTime;

	if (frameTime >= (1.f / 60.f))
	{		
		frameTime = 0.f;

		if (ZSPoses.size() >= maxValueSize)
		{
			ZSPoses.erase(ZSPoses.begin());
			times.erase(times.begin());
		}

		if (!playerASampleDatas.empty())
		{
			ZSPoses.push_back(playerASampleDatas.back().ZuPos);
			times.push_back(playerASampleDatas.back().Time);
		}		
	}

	//std::reverse(ZSPoses.begin(), ZSPoses.end());

	//ZSPoses[0] = 0.1f;
	//ZSPoses[1] = -0.1f;
	

	ImGui::Begin("GraphUI1");
	{
		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

		if (ImPlot::BeginPlot("##Scrolling", ImVec2(-1, 300))) {
			//ImPlot::SetupAxes(nullptr, nullptr, flags, flags);

			//배경움직이기
			ImPlot::SetupAxisLimits(ImAxis_X1, times.back() - 10.f, times.back(), ImGuiCond_Always);

			//ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);			

			//ImPlot::SetupAxisLinks(ImAxis_X1, linkx ? &lims.X.Min : nullptr, linkx ? &lims.X.Max : nullptr);

			double max = 0.0001;
			double min = -0.0001;

			bool bFlag = false;

			for (int i = 0; i < ZSPoses.size(); ++i)
			{
				if (ZSPoses[i] > max)
				{
					max = ZSPoses[i];
					bFlag = true;
				}

				if (ZSPoses[i] < min)
				{
					min = ZSPoses[i];
					bFlag = true;
				}
			}

			//^if (false == bFlag)
			//^{
			//^	max = 0.001;
			//^	min = -0.001;
			//^}

			// 0으로 돌아가려는 성질이있음			


			ImPlot::SetupAxisLinks(ImAxis_Y1, &min, &max);

			ImPlot::PlotLine("Mouse X", times.data(), ZSPoses.data(), ZSPoses.size(), ImPlotFlags_NoFrame);
			
			ImPlot::EndPlot();
		}




	}

	ImGui::PlotLines("Lines", ZSPoses.data(), maxValueSize, 0, nullptr, -0.1f, 0.1f, ImVec2(0, 150.0f));

	//ImGui::PlotLines("ZsPos", funcZsPos, &playerASampleDatas, maxValueSize, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));

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


	innoSimulator->SetMS(MS);
	innoSimulator->SetMU(MU);
	innoSimulator->SetKS(KS);
	innoSimulator->SetCS(CS);
	innoSimulator->SetKT(KT);
	innoSimulator->SetSpeed(Speed);	
	innoSimulator->SetBumpStart(BumpStart);
	innoSimulator->SetBumpEnd(BumpEnd);
	innoSimulator->SetBumpAmp(BumpAmp);
	innoSimulator->SetSamplintTIme(SamplingTime);

#pragma endregion GraphUI1

}
