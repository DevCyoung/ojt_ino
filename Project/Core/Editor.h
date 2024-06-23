#pragma once
void ShowDockSpace();

class Editor
{
public:
	friend int APIENTRY::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow);		

	PUBLIC_SINGLETON_DECLARE(Editor);	

public:
	void CreateDevice(int width, int height);
	void RemoveDevice();
	bool mbInit;
	bool mbRestore;
	bool mbFullScreen;

private:
	void run();
	void present() const;

};

LRESULT WINAPI WndProcImGUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);