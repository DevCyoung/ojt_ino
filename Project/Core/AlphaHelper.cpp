#include "pch.h"
#include "AlphaHelper.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include "BSCollection.h"
#include "RSCollection.h"
#include "RSCollection.h"
#include "DSCollection.h"
#include "Shader.h"
#include "Mesh.h"
#include "ComputeShader.h"
#include "StructBuffer.h"
#include "EnumShaderBindStage.h"
#include "ConstantBuffer.h"
void TextureAlphaTo(Texture* const dst, Texture* const src)
{
	ComputeShader* cs = gResourceManager->Find<ComputeShader>(L"StdCS");
	{
		tMaterial mat;
		mat.arrInt[0] = static_cast<int>(dst->GetWidth());
		mat.arrInt[1] = static_cast<int>(dst->GetHeight());
		mat.arrFloat[0] = 1.0f;

		gGraphicDevice->PassCB(eCBType::Material, sizeof(tMaterial), &mat);
		gGraphicDevice->BindCB(eCBType::Material, eShaderBindType::CS);
	}	
	
	gGraphicDevice->BindUAV(0, src);
	gGraphicDevice->BindUAV(1, dst);
	gGraphicDevice->BindCS(cs);
	gGraphicDevice->Distpatch(cs);

	gGraphicDevice->UnBindUAV(0);
	gGraphicDevice->UnBindUAV(1);
}
