#pragma once
void ShowDockSpace();

class Editor
{
public:
	friend int APIENTRY::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPWSTR lpCmdLine, int nCmdShow);		

	SINGLETON_DECLARE(Editor);	

public:
	bool IsInit() const { return mbInit; }
	bool IsRestore() const { return mbRestore; }
	bool IsFullScreen() const { return mbFullScreen; }
	void SetInit(const bool bInit) { mbInit = bInit; }
	void SetRestore(const bool bRrestorue) { mbRestore = bRrestorue; }

private:
	void createDevice(int width, int height);
	void removeDevice();
	void run();
	void present() const;
private:
	bool mbInit;
	bool mbRestore;
	bool mbFullScreen;
};

LRESULT WINAPI WndProcImGUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);