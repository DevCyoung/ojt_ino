#include "pch.h"
#include "Editor.h"
#include "PanelUIManager.h"
#include "DockSpace.h"
#include "Engine.h"
#include "imgui_theme.h"
#include "implot.h"

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// 전역 변수 선언
LONG g_WindowStyle;
WINDOWPLACEMENT g_wpPrev; // 이전 윈도우 위치와 상태를 저장할 변수

// 윈도우를 전체 화면으로 설정하는 함수
void SetWindowToFullscreen(HWND hWnd)
{
	ZeroMemory(&g_wpPrev, sizeof(WINDOWPLACEMENT));
	g_WindowStyle = 0;

	// 현재 모니터의 해상도를 가져오기
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 이전 윈도우 위치와 상태 저장
	g_WindowStyle = GetWindowLong(hWnd, GWL_STYLE);
	GetWindowPlacement(hWnd, &g_wpPrev);

	// 윈도우 스타일과 위치 설정 (타이틀바 없이 전체 화면)
	SetWindowLong(hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE); // 전체 화면 스타일로 변경
	SetWindowPos(hWnd, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED); // 위치와 크기 조정
}

// 윈도우를 원래 상태로 복구하는 함수
void RestoreWindowFromFullscreen(HWND hWnd)
{
	// 이전 윈도우 스타일 복원
	SetWindowLong(hWnd, GWL_STYLE, g_WindowStyle);

	// 이전 윈도우 위치와 상태 복원
	SetWindowPlacement(hWnd, &g_wpPrev);
	SetWindowPos(hWnd, NULL, g_wpPrev.rcNormalPosition.left, g_wpPrev.rcNormalPosition.top,
		g_wpPrev.rcNormalPosition.right - g_wpPrev.rcNormalPosition.left,
		g_wpPrev.rcNormalPosition.bottom - g_wpPrev.rcNormalPosition.top,
		SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE);
}

void GetScreenResolution(int& width, int& height)
{
	width = GetSystemMetrics(SM_CXSCREEN);
	height = GetSystemMetrics(SM_CYSCREEN);
}

void ToggleFullscreen(HWND hWnd)
{
	//if (!g_pSwapChain)
	//{
	//	//std::cerr << "Swap chain is not initialized." << std::endl;
	//	return;
	//}
	//
	
	//HRESULT hr = g_pSwapChain->SetFullscreenState(!fullscreen, nullptr);
	//
	//if (FAILED(hr))
	//{
	//	//std::cerr << "Failed to toggle fullscreen mode. HRESULT: " << hr << std::endl;
	//}
	//else
	//{
	//	// 성공적으로 전환했음을 알리는 메시지 박스 표시 (선택 사항)
	//	//MessageBox(hWnd, fullscreen ? L"창 모드로 전환되었습니다." : L"전체 화면 모드로 전환되었습니다.", L"알림", MB_OK);
	//}

	//Engine::deleteInstance();
	//Engine::initialize(hWnd, )

	static BOOL fullscreen = true;
	//g_pSwapChain->GetFullscreenState(&fullscreen, nullptr);

	if (fullscreen)
	{
		//Editor::GetInstance()->mbInit = true;
		Editor::GetInstance()->mbInit = true;
	}
	else
	{
		//Editor::GetInstance()->mbInit = true;
		Editor::GetInstance()->mbRestore = true;
	
	}

	fullscreen = !fullscreen;
}

// Forward declarations of helper functions
static bool CreateDeviceD3D(const HWND hWnd, int width, int height);
static void CleanupDeviceD3D();
static void CreateRenderTarget();
static void CleanupRenderTarget();

Editor::Editor()
	: mbInit(false)
	, mbRestore(false)
	, mbFullScreen(false)
{
	CreateDevice(Engine::GetInstance()->mRenderTargetWidth, Engine::GetInstance()->mRenderTargetHeight);

	PanelUIManager::initialize();	
}

Editor::~Editor()
{
	// Cleanup
	RemoveDevice();

	PanelUIManager::deleteInstance();
}

void Editor::CreateDevice(int width, int height)
{
	Engine* const engine = Engine::GetInstance();
	const HWND hwnd = engine->GetHwnd();

	// Initialize Direct3D
	if (!CreateDeviceD3D(hwnd, width, height))
	{
		Assert(false, ASSERT_MSG_INVALID);
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	ImGuiStyle newStyle = ImGuiTheme::ThemeToStyle(ImGuiTheme::ImGuiTheme_::ImGuiTheme_MicrosoftStyle);
	ImGui::GetStyle() = newStyle;

	ImPlot::CreateContext();

}

void Editor::RemoveDevice()
{
	ImPlot::DestroyContext();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();

	ImGui::DestroyContext();

	g_pd3dDevice = nullptr;
	g_pd3dDeviceContext = nullptr;
	g_pSwapChain = nullptr;
	g_SwapChainOccluded = false;
	g_ResizeHeight = 0;
	g_mainRenderTargetView = nullptr;
}

void Editor::run()
{		

	int moniterWidth = GetSystemMetrics(SM_CXSCREEN);
	int moniterHeight = GetSystemMetrics(SM_CYSCREEN);
	int screenWidth = Engine::GetInstance()->mRenderTargetWidth;
	int screenHeight = Engine::GetInstance()->mRenderTargetHeight;
	if (mbInit)
	{
		RemoveDevice();
		SetWindowToFullscreen(Engine::GetInstance()->mHwnd);
		CreateDevice(moniterWidth, moniterHeight);

		mbInit = false;
		mbFullScreen = true;
	}

	if (mbRestore)
	{
		RemoveDevice();
		RestoreWindowFromFullscreen(Engine::GetInstance()->mHwnd);				
		CreateDevice(screenWidth, screenHeight);
		HWND hWnd = Engine::GetInstance()->mHwnd;
		LONG style = GetWindowLong(hWnd, GWL_STYLE);
		style |= WS_CAPTION;
		SetWindowLong(hWnd, GWL_STYLE, style);
		Engine::GetInstance()->setWindowSize(screenWidth, screenHeight);

		mbRestore = false;
		mbFullScreen = false;
	}

	PanelUIManager::GetInstance()->update();

	// Start the Dear ImGui frame
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	
	ImGui::NewFrame();	

	bool show_app_dockspace = true;
	if (show_app_dockspace)
	{
		ShowDockSpace();
	}
	
	PanelUIManager::GetInstance()->finalUpdate();
	PanelUIManager::GetInstance()->render();
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	const float clear_color_with_alpha[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
	g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
	g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}		
}

void Editor::present() const
{        
	// Present
	//HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
	HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
	g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
}


// Helper functions
bool CreateDeviceD3D(const HWND hWnd, int width, int height)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const UINT createDeviceFlags = 0;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	D3D_FEATURE_LEVEL featureLevel;
	HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);

	if (res == DXGI_ERROR_UNSUPPORTED)
	{
		res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
	}
	if (res != S_OK)
	{
		Assert(false, ASSERT_MSG_INVALID);
		return false;
	}

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();

	Assert(g_pSwapChain != nullptr, ASSERT_MSG_NOT_NULL);
	if (g_pSwapChain)
	{
		g_pSwapChain->Release();
		g_pSwapChain = nullptr;
	}

	Assert(g_pd3dDeviceContext != nullptr, ASSERT_MSG_NOT_NULL);
	if (g_pd3dDeviceContext)
	{
		g_pd3dDeviceContext->Release();
		g_pd3dDeviceContext = nullptr;
	}

	Assert(g_pd3dDevice != nullptr, ASSERT_MSG_NOT_NULL);
	if (g_pd3dDevice)
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = nullptr;
	}
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer = nullptr;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	Assert(pBackBuffer, ASSERT_MSG_INVALID);
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	Assert(g_mainRenderTargetView, ASSERT_MSG_NULL);
	if (g_mainRenderTargetView)
	{
		g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr;
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProcImGUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SYSKEYDOWN:
		if (wParam == VK_RETURN && (lParam & (1 << 29))) // Alt + Enter
		{
			ToggleFullscreen(hWnd);
		}
		break;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			return 0;
		g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
		g_ResizeHeight = (UINT)HIWORD(lParam);
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	case WM_DPICHANGED:
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
		{
			const RECT* suggested_rect = (RECT*)lParam;
			::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
		}
		break;
	}
	return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}