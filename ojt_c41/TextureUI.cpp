#include "pch.h"
#include "TextureUI.h"
#include "Texture.h"

void TextureUI(Texture* texture)
{
	ImVec2 renderTargetSize = ImVec2(200.f, 200.f);
	ID3D11ShaderResourceView* rv = texture->GetSRVOrNull();

	ImGui::Image((void*)rv, renderTargetSize);
}
