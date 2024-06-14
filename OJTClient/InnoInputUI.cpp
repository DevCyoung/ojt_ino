#include "pch.h"
#include "InnoInputUI.h"
#include "InnoOJTClient.h"
#include "GameManager.h"
#include "InnoSimulator.h"
#include "InnoDataManager.h"

#include <RenderTargetRenderer.h>
#include <GameSystem.h>
#include <SceneManager.h>
#include <AlphaHelper.h>
#include <Texture.h>
#include <TimeManager.h>
#include <implot.h>
#include <InputManager.h>
#include <LogListUI.h>
#include <PanelUIManager.h>
#include <imgui_internal.h>

#define gLogListUIClient (static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient")))
#define offset 16.f

InnoInputUI::InnoInputUI()
{
	//for (int i = ImGuiTheme::ImGuiTheme_ImGuiColorsClassic; i < ImGuiTheme::ImGuiTheme_Count; ++i)
	//{
	//	vecZsAcc.push_back(ImGuiTheme::ImGuiTheme_Name((ImGuiTheme::ImGuiTheme_)i));
	//}
	//
	//if (ImGui::Combo("combo", &current_item, vecZsAcc))
	//{
	//
	//}

	static int current_item = 7;
	ImGuiStyle newStyle = ImGuiTheme::ThemeToStyle(ImGuiTheme::ImGuiTheme_(current_item));
	ImGuiStyle& style = ImGui::GetStyle();
	style = newStyle;

	ImPlot::CreateContext();
}

InnoInputUI::~InnoInputUI()
{
	ImPlot::DestroyContext();
}

static void ShowGraph(const char* label, const float* values, const float* times, int dataCount, int axxSize = 150)
{	
	std::string key = "##";

	key += label;

	if (ImPlot::BeginPlot(key.c_str(), ImVec2(-1, axxSize)))
	{
		const float history = 10.f;
		double max = 0.0001f;
		double min = -0.0001f;

		for (int i = 0; i < dataCount; ++i)
		{
			if (times[i] < times[dataCount - 1] - history || times[i] > times[dataCount - 1])
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
		ImPlot::SetupAxisLimits(ImAxis_X1, times[dataCount - 1] - history, times[dataCount - 1], ImGuiCond_Always);
		ImPlot::PlotLine(label, times, values, dataCount, ImPlotFlags_NoFrame);
		ImPlot::EndPlot();
	}
}

void IndeterminateProgressBar(const ImVec2& size_arg)
{
	using namespace ImGui;

	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiStyle& style = g.Style;
	ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), g.FontSize + style.FramePadding.y * 2.0f);
	ImVec2 pos = window->DC.CursorPos;
	ImRect bb(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
	ItemSize(size);
	if (!ItemAdd(bb, 0))
		return;

	const float speed = g.FontSize * 0.05f;
	const float phase = ImFmod((float)g.Time * speed, 1.0f);
	const float width_normalized = 0.2f;
	float t0 = phase * (1.0f + width_normalized) - width_normalized;
	float t1 = t0 + width_normalized;

	RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
	bb.Expand(ImVec2(-style.FrameBorderSize, -style.FrameBorderSize));
	RenderRectFilledRangeH(window->DrawList, bb, GetColorU32(ImGuiCol_PlotHistogram), t0, t1, style.FrameRounding);
}

void InnoInputUI::drawForm()
{
	//ImPlot::ShowDemoWindow();
	InnoSimulator* innoSimulator = InnoSimulator::GetInstance();

	const std::vector<float>& vecTimes = InnoDataManager::GetInstance()->GetTimes();
	const std::vector<float>& vecZsPos = InnoDataManager::GetInstance()->GetZsPoses();
	const std::vector<float>& vecZsSpeed = InnoDataManager::GetInstance()->GetZsSpeeds();
	const std::vector<float>& vecZsAcc = InnoDataManager::GetInstance()->GeetZsAccs();
	const std::vector<float>& vecZuPos = InnoDataManager::GetInstance()->GetZuPoses();
	const std::vector<float>& vecZuSpeed = InnoDataManager::GetInstance()->GetZuSpeeds();
	const std::vector<float>& vecZuAcc = InnoDataManager::GetInstance()->GetZuAccs();
	const std::vector<float>& vecZr = InnoDataManager::GetInstance()->GetZrs();
	const std::vector<float>& vecxPos = InnoDataManager::GetInstance()->GetXPoses();
	const std::vector<float>& vecXSpeed = InnoDataManager::GetInstance()->GetXSpeeds();

	GameManager::GetInstance()->mSceneRenderHelperA->Draw(gCurrentScene);
	GameManager::GetInstance()->mSceneRenderHelperB->Draw(gCurrentScene);

	Texture* renderTexA = GameManager::GetInstance()->mSceneRenderHelperA->GetRenderTexture();
	Texture* renderTexB = GameManager::GetInstance()->mSceneRenderHelperB->GetRenderTexture();

	ImVec2 renderTargetSizeA = ImVec2(renderTexA->GetWidth(), renderTexA->GetHeight());
	ImVec2 renderTargetSizeB = ImVec2(renderTexB->GetWidth(), renderTexB->GetHeight());

	std::string button1Name = "Play";

	const eInnoSimulatorState simulatorState = innoSimulator->GetSimulatorState();

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

	static float slidePos = 0.f;
	static float slideMax = 1.f;	

	int dataSize = min(INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND, vecTimes.size());
	int dataPos = 0;


#pragma region InputScreen
	ImGui::Begin("ScreenUI");

	if (ImGui::IsWindowFocused())
	{
		GameManager::GetInstance()->mSceneRenderHelperA->GetCamera()->GetComponent<CameraInputMoveMent>()->MoveCamera();
	}

	ImGui::Image((void*)renderTexA->GetSRV(), renderTargetSizeA);
	//ImGui::Image((void*)renderTexB->GetSRV(), renderTargetSizeB);

	ImGui::Separator();
	ImGui::PushItemWidth(800.f);
	ImGui::SliderFloat("##testFloat", &slidePos, 0.f, slideMax);
	ImGui::PopItemWidth();
	ImGui::Separator();

	if (simulatorState == eInnoSimulatorState::Editing)
	{
		if (ImGui::Button("Stop Editing"))
		{
			innoSimulator->Finish();
		}

		ImGui::SameLine();

		ImGui::Button(">>");

		ImGui::SameLine();

		ImGui::Button("<<");
	}	
	else if (InnoOJTClient::GetInstance()->mServerSocket != INVALID_SOCKET)
	{
		ImGui::Text("Server Trainig mode");
	}
	else if (simulatorState == eInnoSimulatorState::None)
	{
		if (ImGui::Button("Start Training"))
		{
			innoSimulator->Play();
		}
	}	
	else if (simulatorState == eInnoSimulatorState::Playing)
	{
		if (ImGui::Button("Stop Training"))
		{
			innoSimulator->Stop();
		}		
	}		

	ImGui::SameLine(600.f);

	ImGui::Text("Play Time : %.2f / %.2f", vecTimes.back(), vecTimes.back());

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

	ImGui::Text("Speed");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##Speed:InputUI2", &Speed, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("Bump Start");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##BumpStart:InputUI2", &BumpStart, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("Bump End");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##BumpEnd:InputUI2", &BumpEnd, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("Bump Amp");
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);
	ImGui::InputFloat("##BumpAmp:InputUI2", &BumpAmp, 0.f, 0.f);
	ImGui::PopItemWidth();
	ImGui::Spacing();

	ImGui::Text("Sampling Time");
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

	bool bConnecting = InnoOJTClient::GetInstance()->IsConnecting();
	bool bConnected = InnoOJTClient::GetInstance()->IsConnected();

	static char ipBuff[256] = "127.0.0.1";
	static int portNumber = INNO_DEFAULT_PORT;

	if (false == bConnected)
	{		
		ImGui::Text("ServerIP");
		ImGui::SameLine(100.0f);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
		ImGui::PushItemWidth(100.0f);
		ImGui::InputText("##ServerIP", ipBuff, 16);
		ImGui::PopItemWidth();
		ImGui::Spacing();

		ImGui::Text("Port");
		ImGui::SameLine(100.0f);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
		ImGui::PushItemWidth(100.0f);
		ImGui::InputInt("##PortNumber", &portNumber, 0, 0);
		ImGui::PopItemWidth();
		ImGui::Spacing();
	}

	if (bConnecting)
	{
		IndeterminateProgressBar(ImVec2(196.f, 20.f));
	}	
	else if (bConnected)
	{
		std::string serverIP = InnoOJTClient::GetInstance()->GetServerIP();
		int serverPort = InnoOJTClient::GetInstance()->GetServerPort();

		ImGui::Text("Sokcet Connected");
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
		ImGui::Spacing();

		ImGui::Text("Server IP : %s", serverIP.c_str());
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
		ImGui::Spacing();

		ImGui::Text("Server Port : %d", serverPort);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
		ImGui::Spacing();

		// 메시지 박스를 표시합니다.
		
		
		if (ImGui::BeginPopupModal("Client::DisConnect", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure disconnect?");

			ImVec2 button_size(ImGui::GetFontSize() * 7.0f, 0.0f);

			if (ImGui::Button("Yes", button_size))
			{
				InnoOJTClient::GetInstance()->DisConnect();
				ImGui::CloseCurrentPopup();
			}			
			ImGui::SameLine();
			if (ImGui::Button("No", button_size))
			{	
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::Button("DisConnect", ImVec2(196, 20.f)))
		{
			if (!ImGui::IsPopupOpen("Client::DisConnect"))
				ImGui::OpenPopup("Client::DisConnect");
		}
	}
	else
	{		
		if (ImGui::Button("Connect", ImVec2(196, 20.f)))
		{
			char connectBuffer[256] = { 0, };
			sprintf_s(connectBuffer, "Connecting...\nIP : %s\nPort : %d", ipBuff, portNumber);
			gLogListUIClient->WriteLine(connectBuffer);
			InnoOJTClient::GetInstance()->Connect(ipBuff, portNumber);
		}
	}		

	ImGui::End();
#pragma endregion InputUI3

#pragma region GraphUI1
	
	slideMax = vecTimes.back();

	if (simulatorState == eInnoSimulatorState::Playing)
	{
		if (vecTimes.size() >= INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND)
		{
			dataPos = vecTimes.size() - INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND;
		}
	}
	else if (simulatorState == eInnoSimulatorState::Editing)
	{						
		float cp = slidePos / slideMax;
		dataPos = (vecTimes.size() - INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND) * cp;
	}

	if (slideMax <= 0.f)
	{
		dataPos = 0;
	}

	ImGui::Begin("GraphUI1");
	{
		static int axxSize = 150;
		ImGui::SliderInt("##AXXSize", &axxSize, 50, 600);
		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
		ShowGraph("ZsPos", &vecZsPos[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph("ZsSpeed", &vecZsSpeed[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph("ZsAcc", &vecZsAcc[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph("ZuPos", &vecZsPos[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph("ZuSpeed", &vecZuSpeed[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph("ZuAcc", &vecZuAcc[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph("Zr", &vecZr[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph("xPos", &vecxPos[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph("XSpeed", &vecXSpeed[dataPos], &vecTimes[dataPos], dataSize, axxSize);
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
