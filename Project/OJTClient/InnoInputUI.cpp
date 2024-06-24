#include "pch.h"
#include "InnoInputUI.h"
#include "InnoOJTClient.h"
#include "InnoSimulator.h"
#include "InnoDataManager.h"
#include "InnnoSave.h"
#include <implot.h>
#include <imgui_internal.h>
#include <imgui_theme.h>
#include <Editor.h>
#include <InputManager.h>
#define gLogListUIClient (static_cast<LogListUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("LogListUIClient")))
#define offset 16.f

static void ShowBump(int idx, const float yPos, const float bumpStart, const float bumpEnd, ImVec4 bumpColor = ImVec4(255, 0, 0, 255));
static void ShowGraph(const char* label, const float* values, const float* times, int dataCount, int axxSize = 150);
static void ShowInputFloat(const char* label, const char* name, float* pValue, ImGuiInputTextFlags flag, int sameLine = 100, int itemWidth = 100, float cursorOffset = 5.0f, const char* tooltipMessage = nullptr);
static void ShowInputText(const char* label, const char* name, char* pValue, int buffsize, ImGuiInputTextFlags flag);
static void ShowInputInt(const char* label, const char* name, int* pValue, ImGuiInputTextFlags flag);
static void ShowLoadingProgressBar(const ImVec2& size_arg);

InnoInputUI::InnoInputUI()
	:mbSaveClicked(false)
	, mName("who are you?")
{
	SetTitle("InnoInputUI");
}

InnoInputUI::~InnoInputUI()
{
}

void InnoInputUI::drawForm()
{
#pragma region Data


	InnoSimulator* const innoSimulator = InnoSimulator::GetInstance();

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
	const std::vector<float>& vecxOtehrPos = InnoDataManager::GetInstance()->GetXOtherPoses();		

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
	static float editTime = 0.f;
	static int playMode = 0;

	if (simulatorState == eInnoSimulatorState::None)
	{
		slidePos = 0.f;
		editTime = 0.f;
		slideMax = 1.f;
	}

	int dataSize = min(INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND, vecTimes.size());
	int dataPos = 0;

	const bool bConnecting = InnoOJTClient::GetInstance()->IsConnecting();
	const bool bConnected = InnoOJTClient::GetInstance()->IsConnected();

	static char IPBuffer[256] = "127.0.0.1";
	static char ConnectName[256] = "Gest";
	static int portNumber = INNO_DEFAULT_PORT;

	ImGuiInputTextFlags inputTextFlag = 0;
	if (simulatorState != eInnoSimulatorState::None)
	{
		inputTextFlag = ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly;
	}
	if (bConnecting)
	{
		inputTextFlag = ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly;
	}

	//DataPos
	slideMax = vecTimes.back();

	if (slidePos >= slideMax)
	{
		slidePos = slideMax;
		if (playMode > 0)
		{
			playMode = 0;

		}
	}
	if (slidePos < 0.f)
	{
		slidePos = 0.f;
		if (playMode < 0)
		{
			playMode = 0;

		}
	}

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

	if (dataPos < 0.f)
	{
		dataPos = 0;
	}

	static int current_bump_item = 0;
#pragma endregion Data

#pragma region InputScreen
	ImGuiWindowClass window_InputScreen;
	window_InputScreen.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_InputScreen);
	ImGui::Begin("ScreenUI");

	//const bool bFullScreen = Editor::GetInstance()->mbFullScreen;

	const double xHistoryScale = 10.f;
	const float xPos = vecxPos[dataPos + INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND - 1];
	const float xOtherPos = vecxOtehrPos[dataPos + INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND - 1];
	float yPos = 0.f;

	static double timeScale = 10;
	timeScale = InnoSimulator::GetInstance()->InnoSimulator::GetInstance()->timeHistory;


	if (gInput->GetKey(eKeyCode::A))
	{
		timeScale += gDeltaTime * 100;
	}
	else if (gInput->GetKey(eKeyCode::D))
	{
		timeScale -= gDeltaTime * 100;
	}

	double xHistoryMax = timeScale + xPos;
	double xHistoryMin = -timeScale + xPos;
	double yHistoryMax = xHistoryScale;
	double yHistoryMin = -xHistoryScale;

	if (bConnected)
	{
		yPos = 3.f;
	}

	const bool bFullScreen = Editor::GetInstance()->mbFullScreen;
	ImVec2 plotSize = ImVec2(-1, 416);

	if (bFullScreen)
	{
		plotSize = ImVec2(-1, 397);
	}

	if (ImPlot::BeginPlot("##ScreenUI", plotSize))
	{
		float xPosStart = vecxPos[INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND - 1];

		//X스케일 고정
		if (simulatorState != eInnoSimulatorState::None)
		{						
			ImPlot::SetupAxisLinks(ImAxis_X1, &xHistoryMin, &xHistoryMax);
		}

		ImPlot::SetupAxisLinks(ImAxis_Y1, &yHistoryMin, &yHistoryMax);

		//Y축 라벨 지우기
		ImPlot::SetupAxis(ImAxis_::ImAxis_Y1, nullptr, ImPlotAxisFlags_NoTickLabels);

		//Strat Marker
		ImGui::PushID(2);
		ImPlot::SetNextMarkerStyle(2, 10.f, ImVec4(255, 255, 0, 255));
		ImPlot::PlotLine("##StartCarA", &xPosStart, &yPos, 1);
		ImGui::PopID();

		//Start Text
		char xPosStartBuffer[256] = { 0, };
		sprintf_s(xPosStartBuffer, "Start (%.2f)", xPosStart);
		ImPlot::PlotText(xPosStartBuffer, xPosStart, yPos - 1.f);

		//Car move 궤적 자동차 이동흔적
		float xline[2] = { vecxPos[INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND - 1], xPos };
		float yline[2] = { yPos, yPos };
		ImPlot::PlotLine("##CarLine", xline, yline, 2);

		//범프 그리기
		const std::vector<Vector3>& bumpers = InnoSimulator::GetInstance()->GetBumps();
		int itemIdx = current_bump_item;
		for (int i = 0; i < bumpers.size(); ++i)
		{
			if (simulatorState == eInnoSimulatorState::None && current_bump_item == i)
			{
				itemIdx = i;
				continue;
			}
			else
			{
				ShowBump(i, yPos, bumpers[i].x, bumpers[i].y);
			}			
		}

		if (simulatorState == eInnoSimulatorState::None && current_bump_item < bumpers.size())
		{
			ShowBump(itemIdx, yPos, bumpers[current_bump_item].x, bumpers[current_bump_item].y, ImVec4(255, 255, 0, 255));

		}

		//End 위치 그리기
		if (simulatorState == eInnoSimulatorState::Editing)
		{
			ImGui::PushID(2);
			float xEnd = vecxPos.back();
			float yEnd = yPos;
			ImPlot::SetNextMarkerStyle(2, 10.f, ImVec4(0, 255, 255, 255));
			char endBuffer[256];
			sprintf_s(endBuffer, "End (%.2f)", xEnd);
			ImPlot::PlotLine("##End (%.2f)", &xEnd, &yEnd, 1);
			ImPlot::PlotText(endBuffer, xEnd, yEnd - 1.f);
			ImGui::PopID();
		}

		//내차 그리기
		ImGui::PushID(2);
		ImPlot::SetNextMarkerStyle(2, 10.f, ImVec4(255, 0, 255, 255));
		float x = xPos;
		float y = yPos;
		ImPlot::PlotLine("##CarA", &xPos, &y, 1);
		ImGui::PopID();
		char buffMyCar[256] = { 0, };
		sprintf_s(buffMyCar, "My Car (%.2f)", xPos);
		ImPlot::PlotText(buffMyCar, xPos, y + 1.f);


		//OtherCar시작위치
		//OtherCar그리기
		if (bConnected)
		{
			const float xPosOtherStart = vecxOtehrPos[INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND - 1];
			const float x = xOtherPos;
			const float y = -yPos - 3;

			//Other Strat Marker
			ImGui::PushID(2);
			ImPlot::SetNextMarkerStyle(2, 10.f, ImVec4(255, 255, 0, 255));
			ImPlot::PlotLine("##OtherStartCarA", &xPosOtherStart, &y, 1);
			ImGui::PopID();

			//Start Text
			char xPosOtherStartBuffer[256] = { 0, };
			sprintf_s(xPosOtherStartBuffer, "Start (%.2f)", xPosOtherStart);
			ImPlot::PlotText(xPosOtherStartBuffer, xPosOtherStart, y - 1.f);

			//Car 궤적			
			float xline[2] = { vecxOtehrPos[INNO_CLIENT_FRAME_PER_SECOND * INNO_GRAPH_HISTORY_SECOND - 1], x };
			float yline[2] = { y, y };
			ImPlot::PlotLine("##CarLine", xline, yline, 2);

			//End Marker
			if (simulatorState == eInnoSimulatorState::Editing)
			{
				ImGui::PushID(2);
				float xEnd = vecxOtehrPos.back();
				float yEnd = y;
				ImPlot::SetNextMarkerStyle(2, 10.f, ImVec4(0, 255, 255, 255));
				char endBuffer[256];
				sprintf_s(endBuffer, "End (%.2f)", xEnd);
				ImPlot::PlotLine("##End (%.2f)", &xEnd, &yEnd, 1);
				ImPlot::PlotText(endBuffer, xEnd, yEnd - 1.f);
				ImGui::PopID();
			}

			//Other Marker
			ImGui::PushID(2);
			ImPlot::SetNextMarkerStyle(2, 10.f, ImVec4(0, 255, 0, 255));			
			ImPlot::PlotLine("##Other CarA", &x, &y, 1);
			ImGui::PopID();
			char buffMyCar[256] = { 0, };
			sprintf_s(buffMyCar, "Other Car (%.2f)", x);
			ImPlot::PlotText(buffMyCar, x, y + 1.f);
		}

		

		//OtherCar가 화면 밖으로 나갈때 그리기
		if (bConnected)
		{
			float x = xOtherPos;
			if (x < xHistoryMin)
			{
				char buffMyCar[256] = { 0, };
				sprintf_s(buffMyCar, "Other Car (%.2f)", x);
				ImPlot::PlotText(buffMyCar, (xHistoryMax - xHistoryMin) * 0.1f + xHistoryMin, -yPos);
			}
			else if (x > xHistoryMax)
			{
				char buffMyCar[256] = { 0, };
				sprintf_s(buffMyCar, "Other Car (%.2f)", x);
				ImPlot::PlotText(buffMyCar, xHistoryMax - (xHistoryMax - xHistoryMin) * 0.1f, -yPos);
			}
		}

		ImPlot::EndPlot();
	}

	ImGui::Separator();

	ImGui::PushItemWidth(800.f);
	if (simulatorState != eInnoSimulatorState::Editing)
	{
		// 비활성화된 스타일 적용
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	//Editing 중일때만 Slide 활성화
	if (ImGui::SliderFloat("##SlidePos", &slidePos, 0.f, slideMax, "%.2f"))
	{
		//클릭하면 정지상태로 바꿈
		playMode = 0;
	}

	if (simulatorState != eInnoSimulatorState::Editing)
	{
		// 비활성화된 스타일 적용
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopItemFlag();
	}

	ImGui::PopItemWidth();
	ImGui::Separator();



	if (simulatorState == eInnoSimulatorState::Editing)
	{
		if (ImGui::Button("Stop Editing"))
		{
			innoSimulator->Finish();
		}

		ImGui::SameLine();

		if (ImGui::Button(">>"))
		{
			++playMode;
		}

		ImGui::SameLine();

		if (ImGui::Button("<<"))
		{
			--playMode;
		}

		if (playMode > 4)
		{
			playMode = 4;
		}
		if (playMode < -4)
		{
			playMode = -4;
		}

		int moveFrame = 0;

		if (playMode == -4)
		{
			editTime += gDeltaTime * 8;
		}
		else if (playMode == -3)
		{
			editTime += gDeltaTime * 4;
		}
		else if (playMode == -2)
		{
			editTime += gDeltaTime * 2;
		}
		else if (playMode == -1)
		{
			editTime += gDeltaTime;
		}
		else if (playMode == 0)
		{
			//editTime += gDeltaTime;
		}
		else if (playMode == 1)
		{
			editTime += gDeltaTime;
		}
		else if (playMode == 2)
		{
			editTime += gDeltaTime * 2;
		}
		else if (playMode == 3)
		{
			editTime += gDeltaTime * 4;
		}
		else if (playMode == 4)
		{
			editTime += gDeltaTime * 8;
		}

		//editTime += gDeltaTime * moveFrame;

		if (editTime >= INNO_FRAME_DELTA_TIME)
		{
			if (playMode < 0)
			{
				slidePos -= editTime;
			}
			else if (playMode > 0)
			{
				slidePos += editTime;
			}

			editTime = 0.f;
		}
	}
	else if (InnoOJTClient::GetInstance()->mServerSocket != INVALID_SOCKET)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4.f);
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

		slidePos = slideMax;
	}
	if (simulatorState == eInnoSimulatorState::Editing)
	{
		ImGui::SameLine(350.f);
		if (0 == playMode)
		{
			ImGui::Text("Play Stop");
		}
		else if (1 == playMode)
		{
			ImGui::Text("Play");
		}
		else if (2 == playMode)
		{
			ImGui::Text("Play x 2");
		}
		else if (3 == playMode)
		{
			ImGui::Text("Play x 4");
		}
		else if (4 == playMode)
		{
			ImGui::Text("Play x 8");
		}
		else if (-1 == playMode)
		{
			ImGui::Text("Rewind");
		}
		else if (-2 == playMode)
		{
			ImGui::Text("Rewind x 2");
		}
		else if (-3 == playMode)
		{
			ImGui::Text("Rewind x 4");
		}
		else if (-4 == playMode)
		{
			ImGui::Text("Rewind x 8");
		}
	}

	ImGui::SameLine(600.f);
	float curTime = (slidePos / slideMax) * vecTimes.back();
	if (slideMax <= 0.f)
	{
		curTime = slideMax;
	}
	ImGui::Text("Time : %.2f / %.2f", curTime, vecTimes.back());	
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
	float SamplingTime = innoSimulator->GetSamplingTime();
	float StartPos = innoSimulator->GetStartPos();
	static char buff[256] = {};

	ImGuiWindowClass window_InputUI1;
	window_InputUI1.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_InputUI1);
	ImGui::Begin("InputUI1");
	ShowInputFloat("##MS", INNO_MS, &MS, inputTextFlag, 100, 100, 5.0f, INNO_MS_TOOLTIP);
	ShowInputFloat("##MU", INNO_MU, &MU, inputTextFlag, 100, 100, 5.0f, INNO_MU_TOOLTIP);
	ShowInputFloat("##CS", INNO_CS, &CS, inputTextFlag, 100, 100, 5.0f, INNO_CS_TOOLTIP);
	ShowInputFloat("##KS", INNO_KS, &KS, inputTextFlag, 100, 100, 5.0f, INNO_KS_TOOLTIP);
	ShowInputFloat("##KT", INNO_KT, &KT, inputTextFlag, 100, 100, 5.0f, INNO_KT_TOOLTIP);
	//ShowInputFloat("##Start Pos:InputUI2", "Start Pos", &StartPos, inputTextFlag);
	ShowInputFloat("##Speed:InputUI2", INNO_SPEED, &Speed, inputTextFlag, 100, 100, 5.0f, INNO_SPEED_TOOLTIP);
	if (simulatorState == eInnoSimulatorState::Editing)
	{
		ShowInputFloat("##SamplingTime", INNO_SAMPLINT_TIME, &SamplingTime, 0, 100, 100, 5.0f, INNO_SAMPLING_TIME_TOOLTOP);
	}
	else
	{
		ShowInputFloat("##SamplingTime", INNO_SAMPLINT_TIME, &SamplingTime, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly, 100, 100, 5.0f, INNO_SAMPLING_TIME_TOOLTOP);
	}
	ImGui::End();
#pragma endregion InputUI1

#pragma region InputUI2
	ImGuiWindowClass window_InputUI2;
	window_InputUI2.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_InputUI2);
	ImGui::Begin("InputUI2");

	//Bump들 Item으로 만들기
	const std::vector<Vector3> bumps = InnoSimulator::GetInstance()->GetBumps();
	std::vector<std::string> bumpItems;
	for (int i = 0; i < bumps.size(); ++i)
	{
		std::string item;
		item += "Bump ";
		item += std::to_string(i);
		bumpItems.push_back(item);
	}
	const char* listBox[1000] = { "AAA", "BBB", "CCC" };
	for (int i = 0; i < bumpItems.size(); ++i)
	{
		listBox[i] = bumpItems[i].data();
	}	
	ImGui::PushItemWidth(196.f);
	if (ImGui::ListBox("##listbox", &current_bump_item, listBox, bumpItems.size(), 4))
	{	
	}
	ImGui::PopItemWidth();

	//Bump를 삭제하거나 추가한다.
	bool changeFlag = false;
	if (simulatorState == eInnoSimulatorState::None)
	{
		float bumpStart = InnoSimulator::GetInstance()->GetBumpStart();
		float bumpEnd = InnoSimulator::GetInstance()->GetBumpEnd();
		float bumpAmp = InnoSimulator::GetInstance()->GetBumpAmp();

		if (inputTextFlag == 0 && ImGui::Button("Add Bump"))
		{
			InnoSimulator::GetInstance()->PushBump({ bumpStart , bumpEnd, bumpAmp });
			changeFlag = true;
		}

		ImGui::SameLine(107);

		if (inputTextFlag == 0 && ImGui::Button("Remove Bump"))
		{
			if (bumpItems.size() > current_bump_item)
			{
				InnoSimulator::GetInstance()->RemoveBump(current_bump_item);
				changeFlag = true;
			}			
		}
	}

	//선택한 Bump정보 변경
	if (false == changeFlag && bumps.size() > current_bump_item)
	{

		float ShowBumpStart = bumps[current_bump_item].x;
		float ShowBumpEnd = bumps[current_bump_item].y;
		float ShowBumpAmp = bumps[current_bump_item].z;		
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2.f);
		ShowInputFloat("##ShowBumpStart", INNO_BUMP_START, &ShowBumpStart, inputTextFlag, 107, 93, 5.0f, INNO_BUMP_START_TOOLTIP);
		ShowInputFloat("##ShowBumpEnd", INNO_BUMP_END, &ShowBumpEnd, inputTextFlag, 107, 93, 5.0f, INNO_BUMP_END_TOOLTIP);
		ShowInputFloat("##ShowBumpAmp", INNO_BUMP_AMP, &ShowBumpAmp, inputTextFlag, 107, 93, 5.0f, INNO_BUMP_AMP_TOOLTIP);

		InnoSimulator::GetInstance()->SetBump(current_bump_item, Vector3{ ShowBumpStart ,ShowBumpEnd, ShowBumpAmp });
		if (ShowBumpStart < ShowBumpEnd )
		{	
			InnoSimulator::GetInstance()->SetBump(current_bump_item, Vector3{ ShowBumpStart ,ShowBumpEnd, ShowBumpAmp });
		}		
	}
	ImGui::End();
#pragma endregion InputUI2

#pragma region InputUI3
	ImGuiWindowClass window_InputUI3;
	window_InputUI3.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_InputUI3);
	ImGui::Begin("InputUI3");

	if (false == bConnected)
	{
		ShowInputText("##Server IP", "Server IP", IPBuffer, 16, inputTextFlag);
		ShowInputInt("##PortNumber", "Port", &portNumber, inputTextFlag);
		ShowInputText("##Connect Name", "Name", mName, 16, inputTextFlag);

	}

	if (bConnecting)
	{
		ShowLoadingProgressBar(ImVec2(196.f, 20.f));
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
		
		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

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
		if (simulatorState == eInnoSimulatorState::Editing)
		{
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
			ImGui::Text("Please Stop Editing");
		}
		else if (simulatorState == eInnoSimulatorState::Playing)
		{
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
			ImGui::Text("Please Stop Training");
		}
		else if (ImGui::Button("Connect", ImVec2(196, 20.f)))
		{
			char connectBuffer[256] = { 0, };
			sprintf_s(connectBuffer, "Connecting...\nIP : %s\nPort : %d", IPBuffer, portNumber);
			gLogListUIClient->WriteLine(connectBuffer);
			InnoOJTClient::GetInstance()->Connect(IPBuffer, portNumber);
		}
	}

//PopupModalSave
	
	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Client::File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		//ImGui::SetNextWindowPos(ImVec2(300, 300));

		ImVec2 ws = ImGui::GetWindowSize();		
		
		
		ImGui::Text("Select save data");
		static InnnoSave save;
		ImVec2 button_size(ImGui::GetFontSize() * 7.0f, 0.0f);
			
		float of = 120;
		ImGui::Checkbox(ZS_POS, &save.mbZsPos);
		ImGui::SameLine(of);
		ImGui::Checkbox(ZU_POS, &save.mbZuPos);
			
		ImGui::Checkbox(ZS_SPEED, &save.mbZsSpeed);
		ImGui::SameLine(of);
		ImGui::Checkbox(ZU_SPEED, &save.mbZuSpeed);

		ImGui::Checkbox(ZS_ACC, &save.mbZsAcc);
		ImGui::SameLine(of);
		ImGui::Checkbox(ZU_ACC, &save.mbZuAcc);
		
		ImGui::Checkbox(ZR, &save.mbZr);
		ImGui::SameLine(of);
		ImGui::Checkbox(X_POS, &save.mbXPos);

		ImGui::Checkbox(X_SPEED, &save.mbXSpeed);
		ImGui::SameLine(of);
		ImGui::Checkbox(X_POS_OTHER, &save.mbXPosOther);

		if (ImGui::Button("Save", button_size))
		{			
			save.Save();
		}

		ImGui::SameLine(0);

		if (ImGui::Button("No", button_size))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (mbSaveClicked)
	{
		if (!ImGui::IsPopupOpen("Client::File"))
		{
			ImGui::OpenPopup("Client::File");
			mbSaveClicked = false;
		}
	}
	ImGui::End();
#pragma endregion InputUI3

#pragma region GraphUI1
	ImGuiWindowClass window_GraphUI1;
	window_GraphUI1.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
	ImGui::SetNextWindowClass(&window_GraphUI1);
	ImGui::Begin("GraphUI1");
	{
		static int axxSize = 150;
		ImGui::PushItemWidth(441);
		ImGui::SliderInt("##AXXSize", &axxSize, 70, 600);
		ImGui::PopItemWidth();

		static ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;
		ShowGraph(ZS_POS, &vecZsPos[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph(ZS_SPEED, &vecZsSpeed[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph(ZS_ACC, &vecZsAcc[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph(ZU_POS, &vecZuPos[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph(ZU_SPEED, &vecZuSpeed[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph(ZU_ACC, &vecZuAcc[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph(ZR, &vecZr[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph(X_POS, &vecxPos[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		ShowGraph(X_SPEED, &vecXSpeed[dataPos], &vecTimes[dataPos], dataSize, axxSize);

		if (bConnected)
		{
			ShowGraph(X_POS_OTHER, &vecxOtehrPos[dataPos], &vecTimes[dataPos], dataSize, axxSize);
		}

	}
	ImGui::End();
#pragma endregion GraphUI1

#pragma region Set
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
	innoSimulator->SetStartPos(StartPos);
#pragma endregion Set
}

static void ShowBump(int idx, const float yPos, const float bumpStart, const float bumpEnd, ImVec4 bumpColor)
{
	//Bump	
	const float  bumpwidth = 3;
	float bumpX[2] = { bumpStart , bumpEnd };
	float bumpY[2] = { yPos, yPos };
	ImPlot::SetNextLineStyle(bumpColor, bumpwidth);
	ImPlot::PlotLine("##BumpSart-End", bumpX, bumpY, 2, 10);

	//Bump Text Line
	const float textLineOffset = 4.f;

	float bumpStartTextLineX[2] = { bumpStart, bumpStart };
	float bumpStartTextLineY[2] = { yPos, yPos + textLineOffset + (idx % 3) };
	char bumpStartBuffer[256] = { 0, };
	sprintf_s(bumpStartBuffer, "Bump Start %d (%.2f)", idx, bumpStart);
	ImPlot::PlotLine("##BumpStartTextLine", bumpStartTextLineX, bumpStartTextLineY, 2);
	ImPlot::PlotText(bumpStartBuffer, bumpStart, yPos + textLineOffset + 0.5f + (idx % 3));

	float bumpEndTextLineX[2] = { bumpEnd, bumpEnd };
	float bumpEndTextLineY[2] = { yPos, yPos - textLineOffset - (idx % 3) };
	char bumpEndtBuffer[256] = { 0, };
	sprintf_s(bumpEndtBuffer, "Bump End %d (%.2f)", idx, bumpEnd);
	ImPlot::PlotLine("##BumpStartTextLine", bumpEndTextLineX, bumpEndTextLineY, 2);
	ImPlot::PlotText(bumpEndtBuffer, bumpEnd, yPos - textLineOffset - 0.5f - (idx % 3));
}

static void ShowGraph(const char* label, const float* values, const float* times, int dataCount, int axxSize)
{
	std::string key = "##";

	key += label;

	if (ImPlot::BeginPlot(key.c_str(), ImVec2(-1, axxSize)))
	{
		const float history = INNO_GRAPH_HISTORY_SECOND;
		double max = 0.01f;
		double min = -0.01f;

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

		ImPlot::SetupAxisLinks(ImAxis_Y1, &min, &max);
		ImPlot::SetupAxisLimits(ImAxis_X1, times[dataCount - 1] - history, times[dataCount - 1], ImGuiCond_Always);
		ImPlot::PlotLine(label, times, values, dataCount, ImPlotFlags_NoFrame);
		ImPlot::EndPlot();
	}
}

static void ShowInputFloat(const char* label, const char* name, float* pValue, ImGuiInputTextFlags flag, int sameLine, int itemWidth, float cursorOffset, const char* tooltipMessage)
{
	ImGui::Text(name);
	if (nullptr != tooltipMessage && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
	{
		ImGui::SetTooltip(tooltipMessage);
	}

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + cursorOffset);
	ImGui::SameLine(sameLine);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(itemWidth);

	if (flag == ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly)
	{
		// 비활성화된 스타일 적용
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	ImGui::InputFloat(label, pValue, 0.f, 0.f, "%.3f", flag);	

	if (flag == ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly)
	{
		// 스타일 복원
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopItemFlag();
	}


	ImGui::PopItemWidth();
	ImGui::Spacing();
}

static void ShowInputText(const char* label, const char* name, char* pValue, int buffsize, ImGuiInputTextFlags flag)
{
	ImGui::Text(name);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);

	if (flag == ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly)
	{
		// 비활성화된 스타일 적용
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	ImGui::InputText(label, pValue, buffsize, flag);

	if (flag == ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly)
	{
		// 스타일 복원
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopItemFlag();
	}


	ImGui::PopItemWidth();
	ImGui::Spacing();
}

static void ShowInputInt(const char* label, const char* name, int* pValue, ImGuiInputTextFlags flag)
{
	ImGui::Text(name);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
	ImGui::SameLine(100.0f);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::GetFontSize() - offset);
	ImGui::PushItemWidth(100.0f);

	if (flag == ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly)
	{
		// 비활성화된 스타일 적용
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	ImGui::InputInt(label, pValue, 0.f, 0.f, flag);

	if (flag == ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly)
	{
		// 스타일 복원
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopItemFlag();
	}


	ImGui::PopItemWidth();
	ImGui::Spacing();
}

void ShowLoadingProgressBar(const ImVec2& size_arg)
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