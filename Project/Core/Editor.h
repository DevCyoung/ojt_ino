#pragma once
void ShowDockSpace();

class Editor
{
public:
	friend int APIENTRY::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow);		

	SINGLETON_DECLARE(Editor);	

private:
	void run() const;
	void present() const;
};

LRESULT WINAPI WndProcImGUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);