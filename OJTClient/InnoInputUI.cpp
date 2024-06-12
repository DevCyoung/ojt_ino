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
#include <InputManager.h>

InnoInputUI::InnoInputUI()
	: mSceneRenderHelperA(nullptr)
	, mSceneRenderHelperB(nullptr)
{

	constexpr int width = 800;
	constexpr int height = 210;

	mSceneRenderHelperA = new SceneRenderHelper(L"PlayerA", width, height);
	mSceneRenderHelperB = new SceneRenderHelper(L"PlayerB", width, height);

	ImPlot::CreateContext();
}

InnoInputUI::~InnoInputUI()
{
	DELETE_POINTER(mSceneRenderHelperA);
	DELETE_POINTER(mSceneRenderHelperB);

	ImPlot::DestroyContext();
}


#define offset 16.f

static void ShowGraph(const char* label, const std::vector<float>& values, const std::vector<float>& times, int axxSize = 150)
{
	Assert(times.size() == values.size(), ASSERT_MSG_INVALID);

	std::string key = "##";

	key += label;

	if (ImPlot::BeginPlot(key.c_str(), ImVec2(-1, axxSize)))
	{
		const float history = 10.f;
		double max = 0.0001f;
		double min = -0.0001f;

		for (int i = 0; i < values.size(); ++i)
		{
			if (times[i] < times.back() - history || times[i] > times.back())
			{
				continue;
			}

			if (max < values[i])
			{
				max = values[i];
			}

			if (min > values[i])
			{
				min = values[i];
			}
		}

		double size = abs(max) - abs(min);
		size = abs(size);

		max += size * 0.15;
		min -= size * 0.15;

		//invert
		//ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_Invert, 0);

		ImPlot::SetupAxisLinks(ImAxis_Y1, &min, &max);
		ImPlot::SetupAxisLimits(ImAxis_X1, times.back() - history, times.back(), ImGuiCond_Always);
		ImPlot::PlotLine(label, times.data(), values.data(), values.size(), ImPlotFlags_NoFrame);

		ImPlot::EndPlot();
	}
}

void InnoInputUI::drawForm()
{

	static int current_item = 7;
	//for (int i = ImGuiTheme::ImGuiTheme_ImGuiColorsClassic; i < ImGuiTheme::ImGuiTheme_Count; ++i)
	//{
	//	vecZsAcc.push_back(ImGuiTheme::ImGuiTheme_Name((ImGuiTheme::ImGuiTheme_)i));
	//}
	//
	//if (ImGui::Combo("combo", &current_item, vecZsAcc))
	//{
	//
	//}

	{
		ImGuiStyle newStyle = ImGuiTheme::ThemeToStyle(ImGuiTheme::ImGuiTheme_(current_item));
		ImGuiStyle& style = ImGui::GetStyle();
		style = newStyle;
	}

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

	ImGui::SameLine(600.f);

	ImGui::Text("Play Time : 00.00 / 00.00 ");

	//ImGui::Separator();

	ImGui::End();

#pragma endregion InputScreen

#pragma region InputUI1

	float MS = innoSimulator->GetMS();
	float MU = innoSimulator->GetMU();
	float KS = innoSimulator->GetKS();
	float CS = innoSimulator->GetCS();
	float KT = innoSimulator->GetKT();
	float Speed = innoSimulator->GetSpeed();
	float BumpStart = innoSimulator->GetBumpStart();
	float BumpEnd = innoSimulator->GetBumpEnd();
	float BumpAmp = innoSimulator->GetBumpAmp();
	float SamplingTime = innoSimulator->GetSamplintTIme();

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


	const int maxValueSize = 800;
	//마지막 maxValueSize개만 그리면된다.
	const std::vector<tInnoSampleData>& playerASampleDatas = InnoDataManager::GetInstance()->GetPlayerASampleDatas();


	static float frameTime = 0.f;

	static std::vector<float> vecZsPos(maxValueSize, 0.f);
	static std::vector<float> vecZsSpeed(maxValueSize, 0.f);
	static std::vector<float> vecZsAcc(maxValueSize, 0.f);
	static std::vector<float> vecZuPos(maxValueSize, 0.f);
	static std::vector<float> vecZuSpeed(maxValueSize, 0.f);
	static std::vector<float> vecZuAcc(maxValueSize, 0.f);
	static std::vector<float> vecZr(maxValueSize, 0.f);
	static std::vector<float> vecxPos(maxValueSize, 0.f);
	static std::vector<float> vecXSpeed(maxValueSize, 0.f);

	static std::vector<float> vecTimes(maxValueSize, 0.f);

	frameTime += gDeltaTime;

	static bool checker = true;

	if (gInput->GetKeyDown(eKeyCode::SPACE))
	{
		checker = !checker;
	}

	if (checker && frameTime >= (1.f / 60.f))
	{
		frameTime = 0.f;		

		if (vecZsPos.size() >= maxValueSize)
		{
			vecZsPos.erase(vecZsPos.begin());
			vecZsSpeed.erase(vecZsSpeed.begin());
			vecZsAcc.erase(vecZsAcc.begin());
			vecZuPos.erase(vecZuPos.begin());
			vecZuSpeed.erase(vecZuSpeed.begin());
			vecZuAcc.erase(vecZuAcc.begin());
			vecZr.erase(vecZr.begin());
			vecxPos.erase(vecxPos.begin());
			vecXSpeed.erase(vecXSpeed.begin());

			vecTimes.erase(vecTimes.begin()	);
		}

		if (!playerASampleDatas.empty())
		{
			vecZsPos.push_back(playerASampleDatas.back().ZsPos);
			vecZsSpeed.push_back(playerASampleDatas.back().ZsSpeed);
			vecZsAcc.push_back(playerASampleDatas.back().ZsAcc);
			vecZuPos.push_back(playerASampleDatas.back().ZuPos);
			vecZuSpeed.push_back(playerASampleDatas.back().ZuSpeed);
			vecZuAcc.push_back(playerASampleDatas.back().ZuAcc);
			vecZr.push_back(playerASampleDatas.back().Zr);
			vecxPos.push_back(playerASampleDatas.back().xPos);
			vecXSpeed.push_back(playerASampleDatas.back().xSpeed);

			vecTimes.push_back(playerASampleDatas.back().Time);
		}
	}

	ImGui::Begin("GraphUI1");
	{
		static int axxSize = 150;
		ImGui::SliderInt("##AXXSize", &axxSize, 50, 600);


		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
		ShowGraph("ZsPos", vecZsPos, vecTimes  , axxSize);
		ShowGraph("ZsSpeed", vecZsSpeed, vecTimes, axxSize);
		ShowGraph("ZsAcc", vecZsAcc, vecTimes, axxSize);
		ShowGraph("ZuPos", vecZsPos, vecTimes, axxSize);
		ShowGraph("ZuSpeed", vecZuSpeed, vecTimes, axxSize);
		ShowGraph("ZuAcc", vecZuAcc, vecTimes, axxSize);
		ShowGraph("Zr", vecZr, vecTimes, axxSize);
		ShowGraph("xPos", vecxPos, vecTimes, axxSize);
		ShowGraph("XSpeed", vecXSpeed, vecTimes, axxSize);
	}
	ImGui::End();
#pragma endregion GraphUI1

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
}
