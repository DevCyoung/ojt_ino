#include "pch.h"
#include "Animator2D.h"
#include "Animation2D.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"

#include "Material.h"
#include "Shader.h"
#include "CBCollection.h"
#include "StructBuffer.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"
#include "Animation2D.h"

#include "ResourceManager.h"
#include "Mesh.h"
#include "Material.h"

#include "EnumSRV.h"

#include "EngineMath.h"

Animator2D::Animator2D()
	: RenderComponent(eComponentType::Animator2D)
	, mAnimationMap()
	, mEvents()
	, mCurAnimation(nullptr)
	, bRepeat(false)
	, backSize(XMUINT2(300, 300))
	, bFlipX(false)
	, mColorInfo{}
{
	SetMaterial(gResourceManager->FindOrNull<Material>(L"Animation2D"), 0);
	SetMesh(gResourceManager->FindOrNull<Mesh>(L"FillRect2D"));
}

Animator2D::~Animator2D()
{	
	mem::del::DeleteUnorderedMapElements(&mAnimationMap);
}

Animator2D::Animator2D(const Animator2D& other)
	:RenderComponent(other)
{
}

void Animator2D::Play(const std::wstring& animKey, const bool bLoop)
{
	Animation2D* animation = FindAnimationOrNull(animKey);

	Assert(animation, ASSERT_MSG_NULL);

	mCurAnimation = animation;
	bRepeat = bLoop;
	mCurAnimation->reset();
}

void Animator2D::PlayFrame(const std::wstring& animKey, const UINT frameIdx, const bool bLoop)
{
	Animation2D* animation = FindAnimationOrNull(animKey);

	Assert(animation, ASSERT_MSG_NULL);

	mCurAnimation = animation;
	bRepeat = bLoop;
	mCurAnimation->reset(frameIdx);
}

void Animator2D::CreateAnimation(const std::wstring& animName,
	Texture* const atlas,
	const UINT frameCount,
	const XMUINT2 startLeft,
	const XMUINT2 frameSize,
	const XMUINT2 framePad,
	const XMINT2 frameoffset,
	const float duration)
{
	Assert(mAnimationMap.find(animName) == mAnimationMap.end(), ASSERT_MSG_NOT_NULL);

	Animation2D* const animation2D = new Animation2D(animName,
		atlas,
		frameCount,
		startLeft,
		frameSize,
		framePad,
		frameoffset,
		duration);

	mAnimationMap.insert(std::make_pair(animation2D->mKey, animation2D));
}

tCBTransform Animator2D::GetCBTransform() const
{
	tCBTransform CBTransform = {};
	{
		const Vector3& SCALE
			= Vector3(static_cast<float>(backSize.x), static_cast<float>(backSize.y), 1.f);
		const Matrix& SCALE_MATRIX = Matrix::CreateScale(SCALE);


		CBTransform.World = SCALE_MATRIX * GetOwner()->GetComponent<Transform>()->GetWorldMatrix();
	}

	return CBTransform;
}

tCBAnimationInfo Animator2D::GetCBAnimationInfo() const
{
	const Texture* const P_ATLAS = mCurAnimation->GetAtlas();

	Assert(P_ATLAS, ASSERT_MSG_NULL);

	const Animation2D::tFrame& CUR_FRAME = mCurAnimation->GetCurFrame();

	Vector2 atlasUVPerPixel = {};
	atlasUVPerPixel.x = 1.f / static_cast<float>(P_ATLAS->GetWidth());
	atlasUVPerPixel.y = 1.f / static_cast<float>(P_ATLAS->GetHeight());

	//중앙점구하기
	XMINT2 backleftTop = {};

	backleftTop.x = CUR_FRAME.center.x - (backSize.x / 2);
	backleftTop.y = CUR_FRAME.center.y - (backSize.x / 2);

	tCBAnimationInfo CBAnimationInfo = {};
	{
		CBAnimationInfo.UVBackLeftTop.x = static_cast<float>(backleftTop.x) * atlasUVPerPixel.x;
		CBAnimationInfo.UVBackLeftTop.y = static_cast<float>(backleftTop.y) * atlasUVPerPixel.y;

		CBAnimationInfo.UVBackSIze.x = static_cast<float>(backSize.x) * atlasUVPerPixel.x;
		CBAnimationInfo.UVBackSIze.y = static_cast<float>(backSize.y) * atlasUVPerPixel.y;

		CBAnimationInfo.UVLeftTop = CUR_FRAME.UVLeftTop;
		CBAnimationInfo.UVSize = CUR_FRAME.UVSize;

		CBAnimationInfo.UVOffset = CUR_FRAME.UVOffset;
	}

	return CBAnimationInfo;
}

Animation2D* Animator2D::FindAnimationOrNull(const std::wstring& animName) const
{
	auto iter = mAnimationMap.find(animName);

	if (iter == mAnimationMap.end())
	{
		return nullptr;
	}

	return iter->second;
}

Animator2D::Events* Animator2D::FindEvents(const std::wstring animName) const
{
	auto iter = mEvents.find(animName);

	Assert(iter != mEvents.end(), ASSERT_MSG_NULL);

	return iter->second;
}

const std::function<void()>& Animator2D::FindStartEvent(const std::wstring animName)
{
	Events* events = FindEvents(animName);

	return events->startEvent.mEvent;
}

const std::function<void()>& Animator2D::FindCompleteEvent(const std::wstring animName)
{
	Events* events = FindEvents(animName);

	return events->completeEvent.mEvent;
}

const std::function<void()>& Animator2D::FindEndEvent(const std::wstring animName)
{
	Events* events = FindEvents(animName);

	return events->endEvent.mEvent;
}

void Animator2D::SetColorR(float r)
{
	mColorInfo.bSetColorR = 1;
	mColorInfo.R = r;
}

void Animator2D::SetColorG(float g)
{
	mColorInfo.bSetColorG = 1;
	mColorInfo.G = g;
}

void Animator2D::SetColorB(float b)
{
	mColorInfo.bSetColorB = 1;
	mColorInfo.B = b;
}

void Animator2D::SetColorA(float a)
{
	mColorInfo.bSetColorA = 1;
	mColorInfo.A = a;	
}

void Animator2D::SetColorReset()
{
	mColorInfo.bSetColorR = 0;
	mColorInfo.bSetColorG = 0;
	mColorInfo.bSetColorB = 0;
	mColorInfo.bSetColorA = 0;
}

void Animator2D::MulColorR(float r)
{
	mColorInfo.bMulColorR = 1;
	mColorInfo.MulR = r;
}

void Animator2D::MulColorG(float g)
{
	mColorInfo.bMulColorG = 1;
	mColorInfo.MulG = g;
}

void Animator2D::MulColorB(float b)
{
	mColorInfo.bMulColorB = 1;
	mColorInfo.MulB = b;
}

void Animator2D::MulColorA(float a)
{
	mColorInfo.bMulColorA = 1;
	mColorInfo.MulA = a;
}

void Animator2D::MulColor(float r, float g, float b, float a)
{
	MulColorR(r);
	MulColorG(g);
	MulColorB(b);
	MulColorA(a);
}

void Animator2D::MulColorReset()
{
	mColorInfo.bMulColorR = 0;
	mColorInfo.bMulColorG = 0;
	mColorInfo.bMulColorB = 0;
	mColorInfo.bMulColorA = 0;
}


void Animator2D::initialize()
{
}

void Animator2D::update()
{
	RenderComponent::update();
}

void Animator2D::lateUpdate()
{
	if (nullptr == mCurAnimation)
	{
		return;
	}

	if (bRepeat && mCurAnimation->mbFinished)
	{
		mCurAnimation->reset();
	}

	mCurAnimation->lateUpdate();
}

void Animator2D::render(const Matrix& viewMatrix, const Matrix& projectionMatrix)
{
	Assert(mMesh, ASSERT_MSG_NULL);
	Assert(mMaterials[0], ASSERT_MSG_NULL);
	//Assert(camera, ASSERT_MSG_NULL);

	if (nullptr == mCurAnimation)
	{
		return;
	}

	tCBTransform CBTransform = {};
	{
		const Vector3& SCALE
			= Vector3(static_cast<float>(backSize.x), static_cast<float>(backSize.y), 1.f);
		const Matrix& SCALE_MATRIX = Matrix::CreateScale(SCALE);

		if (bFlipX)
		{
			Matrix rotationMatrix = {};
			rotationMatrix = Matrix::CreateRotationZ(Deg2Rad(0.f));
			rotationMatrix *= Matrix::CreateRotationY(Deg2Rad(180));
			rotationMatrix *= Matrix::CreateRotationX(Deg2Rad(0.f));

			CBTransform.World = SCALE_MATRIX * rotationMatrix * GetOwner()->GetComponent<Transform>()->GetWorldMatrix();
		}
		else
		{
			CBTransform.World = SCALE_MATRIX * GetOwner()->GetComponent<Transform>()->GetWorldMatrix();
		}

		
		CBTransform.View = viewMatrix;
		CBTransform.Proj = projectionMatrix;

		gGraphicDevice->PassCB(eCBType::Transform, sizeof(CBTransform), &CBTransform);
		gGraphicDevice->BindCB(eCBType::Transform, eShaderBindType::VS);
	}

	tCBColorInfo CBColorInfo = mColorInfo;
	{
		gGraphicDevice->PassCB(eCBType::ColorInfo, sizeof(CBColorInfo), &CBColorInfo);
		gGraphicDevice->BindCB(eCBType::ColorInfo, eShaderBindType::PS);
	}

	gGraphicDevice->BindMesh(mMesh, 0);
	gGraphicDevice->BindIA(mMaterials[0]->GetShader());
	gGraphicDevice->BindPS(mMaterials[0]->GetShader());
	gGraphicDevice->BindVS(mMaterials[0]->GetShader());
	gGraphicDevice->BindBS(mMaterials[0]->GetShader()->GetBSType());
	gGraphicDevice->BindDS(mMaterials[0]->GetShader()->GetDSType());
	gGraphicDevice->BindRS(mMaterials[0]->GetShader()->GetRSType());


	const Texture* const P_ATLAS = mCurAnimation->GetAtlas();
	Assert(P_ATLAS, ASSERT_MSG_NULL);

	tCBAnimationInfo CBAnimationInfo = GetCBAnimationInfo();			
	gGraphicDevice->PassCB(eCBType::Animation2DInfo, sizeof(CBAnimationInfo), &CBAnimationInfo);
	gGraphicDevice->BindCB(eCBType::Animation2DInfo, eShaderBindType::PS);

	gGraphicDevice->BindSRV(eShaderBindType::PS, static_cast<UINT>(eSRVTpye::AtlasAnimation2D), P_ATLAS);

	gGraphicDevice->Draw(mMesh, 0);
}
