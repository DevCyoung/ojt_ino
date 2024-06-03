#include "pch.h"
#include "LogUI.h"
#include "PanelUIManager.h"

static ExampleAppLog mAppLog;

LogUI::LogUI()
{
    SetTitle("Log");
}

void LogUI::drawForm()
{
    //static ExampleAppLog log;

    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to the _same_ window.
    // Most of the contents of the window will be added by the log.Draw() call.
    bool p_open = true;

    //ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    //ImGui::Begin("Example: Log", &p_open);
    ////IMGUI_DEMO_MARKER("Examples/Log");
    //if (ImGui::SmallButton("[Debug] Add 5 entries"))
    //{
    //    static int counter = 0;
    //    const char* categories[3] = { "info", "warn", "error" };
    //    const char* words[] = { "Bumfuzzled", "Cattywampus", "Snickersnee", "Abibliophobia", "Absquatulate", "Nincompoop", "Pauciloquent" };
    //    for (int n = 0; n < 5; n++)
    //    {
    //        const char* category = categories[counter % IM_ARRAYSIZE(categories)];
    //        const char* word = words[counter % IM_ARRAYSIZE(words)];
    //        log.AddLog("[%05d] [%s] Hello, current time is %.1f, here's a word: '%s'\n",
    //            ImGui::GetFrameCount(), category, ImGui::GetTime(), word);
    //        counter++;
    //    }
    //}
    //ImGui::End();

    // Actually call in the regular Log helper (which will Begin() into the same window as we just did)
    //mAppLog.AddLog("this is log");
    mAppLog.Draw("EL##LogUI", &p_open);
    
}

//void LogUI::AddLog(const char* fmt, ...)
//{
//    mAppLog.AddLog("this is log");
//}

void LogUI::Log(const char* fmt, ...)
{
    int old_size = mAppLog.Buf.size();
    va_list args;
    va_start(args, fmt);
    mAppLog.Buf.appendfv(fmt, args);
    va_end(args);
    for (int new_size = mAppLog.Buf.size(); old_size < new_size; old_size++)
        if (mAppLog.Buf[old_size] == '\n')
            mAppLog.LineOffsets.push_back(old_size + 1);
}
