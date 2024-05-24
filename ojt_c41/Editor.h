#pragma once

//#include <Helper/>

//class PanelUI;

class Editor
{
	friend int APIENTRY::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow);	

	SINGLETON_DECLARE(Editor);	

private:
	void run();
};

LRESULT WINAPI WndProcImGUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);