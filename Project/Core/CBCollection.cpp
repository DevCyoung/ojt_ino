#include "pch.h"
#include "CBCollection.h"
#include "StructBuffer.h"


CBCollection::CBCollection(ID3D11Device* const device)
	: mConstantBuffers
	{
		ConstantBuffer(eCBType::Transform, sizeof(tCBTransform), device),
		ConstantBuffer(eCBType::ColorInfo, sizeof(tCBColorInfo), device),
		ConstantBuffer(eCBType::DebugInfo, sizeof(tCBDebugInfo), device),
		ConstantBuffer(eCBType::Animation2DInfo, sizeof(tCBAnimationInfo), device),
		ConstantBuffer(eCBType::GlobalInfo, sizeof(tGlobalInfo), device),
		ConstantBuffer(eCBType::Sprite2D, sizeof(tSprite2DInfo), device),
		ConstantBuffer(eCBType::Wave, sizeof(tWaveInfo), device),
		ConstantBuffer(eCBType::Material, sizeof(tMaterial), device),
		ConstantBuffer(eCBType::Light3D, sizeof(tLight3DInfo), device),
		ConstantBuffer(eCBType::GraphicDebug, sizeof(tGraphicDebug), device)
	}
{
}

CBCollection::~CBCollection()
{
}
		