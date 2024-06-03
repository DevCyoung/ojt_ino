#include "pch.h"
#include "Light3D.h"
#include "CBCollection.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include  "Transform.h"
#include "SceneManager.h"
#include "GameSystem.h"
#include "RenderTargetRenderer.h"
#include "DebugRenderer2D.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "InputManager.h"
Light3D::Light3D()
	: Component(eComponentType::Light3D)
	, mLightInfo{}
{
	tLight3DInfo info = {};
	info.LightDir = { 1.f, -1.f, 1.f };
	info.LightColor = { 1.f, 1.f, 1.f };
	info.LightSpecCoeff = 0.3f;
	info.LightAmb = { 0.15f, 0.15f, 0.15f };

	mLightInfo = info;
}

Light3D::~Light3D()
{
}

void Light3D::initialize()
{
}

void Light3D::update()
{	
}

void Light3D::lateUpdate()
{
	//mLightInfo.LightDir

	Vector3 f = GetComponent<Transform>()->GetForward();
	mLightInfo.LightDir = Vector4(f.x, f.y, f.z, 0.f);

	gGraphicDevice->PassCB(eCBType::Light3D, sizeof(mLightInfo), &mLightInfo);
	gGraphicDevice->BindCBAllShader(eCBType::Light3D);

	Matrix word = GetComponent<Transform>()->GetWorldMatrix();
	gCurrentSceneRenderer->GetDebugRenderer2D()->DrawCube3D( word, 0 , Vector4(1.f, 1.f, 0.f, 1.f));

	const FLOAT color[4] = { 0.5f, 0.5f, 0.5f, 1.f };

	//FIXME
	static tGraphicDebug graphicDebug = {};

	if (gInput->GetKeyDown(eKeyCode::NUM7))
	{
		graphicDebug.Debug0 ^= 1;
	}
	else if (gInput->GetKeyDown(eKeyCode::NUM8))
	{
		graphicDebug.Debug1 ^= 1;
	}
	else if (gInput->GetKeyDown(eKeyCode::NUM9))
	{
		graphicDebug.Debug2 ^= 1;
	}
	else if (gInput->GetKeyDown(eKeyCode::NUM0))
	{
		graphicDebug.Debug3 ^= 1;
	}	


	gCurrentSceneRenderer->SetGraphicDebug(graphicDebug);

	//Depth
	Texture* tex = gResourceManager->Find<Texture>(L"ShadowMap_0");
	gGraphicDevice->ClearRenderTarget(
		nullptr,
		tex->GetDSV(), color);

	gCurrentSceneRenderer->RenderShadowMap(GetComponent<Transform>(),
		static_cast<UINT>(tex->GetWidth()),
		static_cast<UINT>(tex->GetHeight()),
		tex->GetDSV());
}
