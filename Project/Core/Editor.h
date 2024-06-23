#pragma once
void ShowDockSpace();

class Editor
{
public:
	friend int APIENTRY::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow);		

	SINGLETON_DECLARE(Editor);	
public:
	bool mbInit;
	bool mbRestore;
	bool mbFullScreen;
	void CreateDevice(int width, int height);
	void run();
	void RemoveDevice();
	void present() const;
};

LRESULT WINAPI WndProcImGUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);