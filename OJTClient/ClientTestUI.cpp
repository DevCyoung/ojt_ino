#include "pch.h"
#include "ClientTestUI.h"
#include <iostream>
#include "InnoOJTClient.h"
#include <EngineFile.h>
#include "CarSimulation.h"
#include "InnoDataManager.h"
ClientTestUI::ClientTestUI()
{
	SetTitle("ClientTestUI");
}

ClientTestUI::~ClientTestUI()
{
}

// 서버로부터 메시지 수신하는 함수
void receive_messages(SOCKET ConnectSocket) {
	char recvbuf[INNO_MAX_PACKET_SIZE];
	int recvbuflen = INNO_MAX_PACKET_SIZE;

	while (true) {
		auto start = std::chrono::high_resolution_clock::now();  // 시작 시간 기록
		int bytesReceived = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		auto end = std::chrono::high_resolution_clock::now();  // 종료 시간 기록
		if (bytesReceived > 0) {
			//std::cout << "Received: " << std::string(recvbuf, 0, bytesReceived) << "\n";
			std::chrono::duration<double> elapsed = end - start;
		}
		else if (bytesReceived == 0) {
			//std::cout << "Connection closed by server.\n";
			break;
		}
		else {
			//std::cerr << "recv failed.\n";
			break;
		}	
	}

	closesocket(ConnectSocket);
	WSACleanup();
}


void ClientTestUI::drawForm()
{
	ImGui::Begin("ClientTestUI");
	ImGui::Text("hello world!");

	static bool bConnect = false;

	static float startPos = 0.f;
	static float direction = 1.f;

	if (ImGui::InputFloat("star pos", &startPos))
	{
		InnoOJTClient::GetInstance()->mCurPos[0] = startPos;
	}

	if (ImGui::InputFloat("directtion", &direction))
	{
		InnoOJTClient::GetInstance()->mCarDirection = direction;
	}

	//startPos
	

	//Direction

	if (!bConnect && ImGui::Button("Connect"))
	{
		InnoOJTClient::GetInstance()->Connect("127.0.0.1", 5400);
		bConnect = true;
	}

	static char buff[256] = { 0, };

	ImGui::InputText("##asd", buff, 256);

	if (ImGui::Button("test log"))
	{
		InnoOJTClient::GetInstance()->SendLog(strlen(buff), buff);
	}

	if (ImGui::Button("test Pos"))
	{
		InnoOJTClient::GetInstance()->SendPos(4.645f);
	}

    static int func_type = 0, display_count = 10000;



    struct Funcs
    {
        static float ZsPos(const std::vector<tSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZsPos; }
        static float ZsSpeed(const std::vector<tSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZsSpeed;}
        static float ZsAcc(const std::vector<tSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZsAcc; }
        static float ZuPos(const std::vector<tSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZuPos; }
        static float ZuSpeed(const std::vector<tSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZuSpeed; }
        static float ZuAcc(const std::vector<tSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].ZuAcc; }
        static float Zr(const std::vector<tSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].Zr; }
        static float XPos(const std::vector<tSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].xPos; }
        static float XSpeed(const std::vector<tSampleData>* playerASampleDatas, int i) { return (*playerASampleDatas)[i].xSpeed; }
        //static float Saw(void*, int i) { return (i & 1) ? 1.0f : -1.0f; }
    };

    float (*funcZsPos)(void*, int) = (float (*)(void*, int))Funcs::ZsPos;
    float (*funcZsSpeed)(void*, int) = (float (*)(void*, int))Funcs::ZsSpeed;
    float (*funcZsAcc)(void*, int) = (float (*)(void*, int))Funcs::ZsAcc;
    float (*funcZuPos)(void*, int) = (float (*)(void*, int))Funcs::ZuPos;
    float (*funcZuSpeed)(void*, int) = (float (*)(void*, int))Funcs::ZuSpeed;
    float (*funcZuAcc)(void*, int) = (float (*)(void*, int))Funcs::ZuAcc;
    float (*funcZr)(void*, int) = (float (*)(void*, int))Funcs::Zr;

    //float (*funcXPos)(void*, int) = (float (*)(void*, int))Funcs::XPos;
    //float (*funcXSpeed)(void*, int) = (float (*)(void*, int))Funcs::XSpeed;



    //if (CarSimulation.)

    const float GRAPH_MIN = -0.05f;
    const float GRAPH_MAX = 0.05f;
    const float GRAPH_HEIGHT = 100.f;

    if (InnoDataManager::GetInstance()->GetPlayerASampleDatas().size() >= 70)
    {
        std::vector<tSampleData>& playerASampleDatas = (std::vector<tSampleData>&)InnoDataManager::GetInstance()->GetPlayerASampleDatas();

        ImGui::PlotLines("ZsPos", funcZsPos, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotLines("ZsSpeed", funcZsSpeed, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotLines("ZsAcc", funcZsAcc, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotLines("ZuPos", funcZuPos, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotLines("ZuSpeed", funcZuSpeed, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotLines("ZuAcc", funcZuAcc, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotLines("Zr", funcZr, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotHistogram("ZsPos", funcZsPos, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotHistogram("ZsSpeed", funcZsSpeed, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotHistogram("ZsAcc", funcZsAcc, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotHistogram("ZuPos", funcZuPos, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotHistogram("ZuSpeed", funcZuSpeed, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotHistogram("ZuAcc", funcZuAcc, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));
        ImGui::PlotHistogram("Zr", funcZr, &playerASampleDatas, display_count, 0, NULL, GRAPH_MIN, GRAPH_MAX, ImVec2(0, GRAPH_HEIGHT));

        //ImGui::PlotLines("XPos", func, &mCarSumulation, display_count, 0, NULL, -0.05f, 0.05f, ImVec2(0, 100));
        //ImGui::PlotLines("XSpeed", func, &mCarSumulation, display_count, 0, NULL, -0.05f, 0.05f, ImVec2(0, 100));		
    }

    if (ImGui::Button("test dialog"))
    {
        helper::DialogPath dialogpath = helper::SaveDialog();
        mCarSumulation.SaveData(dialogpath.path.c_str());

    }

    if (ImGui::Button("Sim Test"))
    {
        for (int i = 0; i < 10000; ++i)
        {
            mCarSumulation.Update();
        }

        mCarSumulation.GetCS();
    }
	//ImGui::drawpoint

	

    if (ImGui::BeginTabBar("##TabBar"))
    {

        if (ImGui::BeginTabItem("Canvas"))
        {
            static ImVector<ImVec2> points;
            static ImVec2 scrolling(0.0f, 0.0f);
            static bool opt_enable_grid = true;
            static bool opt_enable_context_menu = true;
            static bool adding_line = false;

            ImGui::Checkbox("Enable grid", &opt_enable_grid);
            ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
            ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

            // Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
            // Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
            // To use a child window instead we could use, e.g:
            //      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
            //      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
            //      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Border, ImGuiWindowFlags_NoMove);
            //      ImGui::PopStyleColor();
            //      ImGui::PopStyleVar();
            //      [...]
            //      ImGui::EndChild();

            // Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
            ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
            ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
            if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
            if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
            ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

            // Draw border and background color
            ImGuiIO& io = ImGui::GetIO();
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
            draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));




            // This will catch our interactions
            ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
            const bool is_hovered = ImGui::IsItemHovered(); // Hovered
            const bool is_active = ImGui::IsItemActive();   // Held
            const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
            const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

            // Add first and second point
            if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
            {
                points.push_back(mouse_pos_in_canvas);
                points.push_back(mouse_pos_in_canvas);
                adding_line = true;
            }
            if (adding_line)
            {
                points.back() = mouse_pos_in_canvas;
                if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                    adding_line = false;
            }

            // Pan (we use a zero mouse threshold when there's no context menu)
            // You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
            const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
            if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
            {
                scrolling.x += io.MouseDelta.x;
                scrolling.y += io.MouseDelta.y;
            }

            // Context menu (under default mouse threshold)
            ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
            if (opt_enable_context_menu && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
                ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
            if (ImGui::BeginPopup("context"))
            {
                if (adding_line)
                    points.resize(points.size() - 2);
                adding_line = false;
                if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
                if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
                ImGui::EndPopup();
            }

            // Draw grid + all lines in the canvas
            draw_list->PushClipRect(canvas_p0, canvas_p1, true);
            if (opt_enable_grid)
            {
                const float GRID_STEP = 64.0f;
                for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
                    draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
                for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
                    draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
            }

            //draw_list->AddRectFilled(ImVec2(27.f, 41.f), ImVec2(100.f, 100.f), IM_COL32(255, 0, 0, 255));


            for (int n = 0; n < points.Size; n += 2)
                draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);

            if (InnoOJTClient::GetInstance()->mbServerTraining)
            {
                float pos1 = InnoOJTClient::GetInstance()->mCurPos[0];
                float pos2 = InnoOJTClient::GetInstance()->mCurPos[1];

                draw_list->AddLine(
                    ImVec2(origin.x + pos1,         origin.y + 20.f),
                    ImVec2(origin.x + pos1 + 20.f,  origin.y + 20.f), IM_COL32(255, 255, 255, 255), 16.f);

                draw_list->AddLine(
                    ImVec2(origin.x + pos2,         origin.y + 40.f),
                    ImVec2(origin.x + pos2 + 20.f,  origin.y + 40.f), IM_COL32(0, 255, 0, 255), 16.f);
            }
            else
            {
                draw_list->AddLine(
                    ImVec2(origin.x + 20.f, origin.y + 20.f),
                    ImVec2(origin.x + 40.f, origin.y + 20.f), IM_COL32(255, 255, 255, 255), 16.f);

                draw_list->AddLine(
                    ImVec2(origin.x + 820.f, origin.y + 40.f),
                    ImVec2(origin.x + 840.f, origin.y + 40.f), IM_COL32(0, 255, 0, 255), 16.f);
            }



            draw_list->PopClipRect();

            ImGui::EndTabItem();
        }

        /*if (ImGui::BeginTabItem("BG/FG draw lists"))
        {
            static bool draw_bg = true;
            static bool draw_fg = true;
            ImGui::Checkbox("Draw in Background draw list", &draw_bg);
            ImGui::SameLine(); HelpMarker("The Background draw list will be rendered below every Dear ImGui windows.");
            ImGui::Checkbox("Draw in Foreground draw list", &draw_fg);
            ImGui::SameLine(); HelpMarker("The Foreground draw list will be rendered over every Dear ImGui windows.");
            ImVec2 window_pos = ImGui::GetWindowPos();
            ImVec2 window_size = ImGui::GetWindowSize();
            ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);
            if (draw_bg)
                ImGui::GetBackgroundDrawList()->AddCircle(window_center, window_size.x * 0.6f, IM_COL32(255, 0, 0, 200), 0, 10 + 4);
            if (draw_fg)
                ImGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * 0.6f, IM_COL32(0, 255, 0, 200), 0, 10);
            ImGui::EndTabItem();
        }*/

        // Demonstrate out-of-order rendering via channels splitting
        // We use functions in ImDrawList as each draw list contains a convenience splitter,
        // but you can also instantiate your own ImDrawListSplitter if you need to nest them.
        if (ImGui::BeginTabItem("Draw Channels"))
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            {
                ImGui::Text("Blue shape is drawn first: appears in back");
                ImGui::Text("Red shape is drawn after: appears in front");
                ImVec2 p0 = ImGui::GetCursorScreenPos();
                draw_list->AddRectFilled(ImVec2(p0.x, p0.y), ImVec2(p0.x + 50, p0.y + 50), IM_COL32(0, 0, 255, 255)); // Blue
                draw_list->AddRectFilled(ImVec2(p0.x + 25, p0.y + 25), ImVec2(p0.x + 75, p0.y + 75), IM_COL32(255, 0, 0, 255)); // Red
                ImGui::Dummy(ImVec2(75, 75));
            }
            ImGui::Separator();
            {
                ImGui::Text("Blue shape is drawn first, into channel 1: appears in front");
                ImGui::Text("Red shape is drawn after, into channel 0: appears in back");
                ImVec2 p1 = ImGui::GetCursorScreenPos();

                // Create 2 channels and draw a Blue shape THEN a Red shape.
                // You can create any number of channels. Tables API use 1 channel per column in order to better batch draw calls.
                draw_list->ChannelsSplit(2);
                draw_list->ChannelsSetCurrent(1);
                draw_list->AddRectFilled(ImVec2(p1.x, p1.y), ImVec2(p1.x + 50, p1.y + 50), IM_COL32(0, 0, 255, 255)); // Blue
                draw_list->ChannelsSetCurrent(0);
                draw_list->AddRectFilled(ImVec2(p1.x + 25, p1.y + 25), ImVec2(p1.x + 75, p1.y + 75), IM_COL32(255, 0, 0, 255)); // Red

                // Flatten/reorder channels. Red shape is in channel 0 and it appears below the Blue shape in channel 1.
                // This works by copying draw indices only (vertices are not copied).
                draw_list->ChannelsMerge();
                ImGui::Dummy(ImVec2(75, 75));
                ImGui::Text("After reordering, contents of channel 0 appears below channel 1.");
            }
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
	ImGui::End();

	//ImGui::ShowDemoWindow();


}
