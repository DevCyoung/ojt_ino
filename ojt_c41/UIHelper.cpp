#include "pch.h"
#include "UIHelper.h"
#include "Texture.h"
#include "InputManager.h"
#include "ResourceManager.h"

namespace ImGui
{
	void DrawTextrue(const Texture& textrue, const Vector2& size)
	{	
		(void)textrue;
		(void)size;
		//ID3D11ShaderResourceView* renderTexView = textrue.GetID3D11ShaderResourceView();
		//ImGui::Image((void*)renderTexView, ImVec2(size.x, size.y));
	}
}
