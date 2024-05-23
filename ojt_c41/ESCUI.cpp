#include "pch.h"
#include "ESCUI.h"
#include "Texture.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "UIHelper.h"

void ESCUI::drawForm()
{
	static float f = 0.0f;
	static int counter = 0;

	Texture* renderTex = ResourceManager::GetInstance()->Find<Texture>(L"CopyRenderTargetTexture");
	ID3D11ShaderResourceView* renderTexView = renderTex->GetSRV();

	ImGui::Begin("DirectX11 Texture Test");
	ImGui::Text("pointer = %p", renderTexView);
	ImGui::Text("size = %d x %d", renderTex->GetWidth(), renderTex->GetHeight());
	ImGui::Image((void*)renderTexView, ImVec2(renderTex->GetWidth(), renderTex->GetHeight()));
	//ImGui::DrawTextrue(*Texture, )
	ImGui::End();
}

void ESCUI::update()
{
	if (gInput->GetKeyDown(eKeyCode::Q))
	{
		TurnOffVisible();
	}

	if (gInput->GetKeyDown(eKeyCode::W))
	{
		TurnOnVisible();
	}
}
