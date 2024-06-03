#include "pch.h"
#include "RenderTargetRenderer.h"
#include "DebugRenderer2D.h"
#include "Transform.h"
#include "Camera.h"
#include "RenderComponent.h"
#include "Light2D.h"
#include "StructuredBuffer.h"
#include "Material.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include "Transform.h"

#include "EnumShaderBindStage.h"
#include "CBCollection.h"
#include "StructBuffer.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "Texture.h"

#include "InputManager.h"
RenderTargetRenderer::RenderTargetRenderer()
	: mDebugRenderer(new DebugRenderer2D())
	, mCameras{ 0, }
	, mRenderComponentsArray{}
	, mLight2DInfos()
	, mbDebugRender(true)
	, mbWireFrame(false)
	, mCameraMask(0XFFFFFFFF)
	, mGraphicDebug{}
{
	for (auto& renderObjectArray : mRenderComponentsArray)
	{
		renderObjectArray.reserve(100);
	}

	mPostProcessComponents.reserve(100);
}

RenderTargetRenderer::~RenderTargetRenderer()
{
	DELETE_POINTER_NOT_NULL(mDebugRenderer);
}

void RenderTargetRenderer::RegisterRenderCamera(Camera* const camera)
{
	const eCameraPriorityType CAMERA_PRIORITY_TYPE = camera->GetPriorityType();

	Assert(camera, ASSERT_MSG_NULL);
	Assert(CAMERA_PRIORITY_TYPE != eCameraPriorityType::End, ASSERT_MSG_INVALID);
	Assert(!mCameras[static_cast<UINT>(CAMERA_PRIORITY_TYPE)], ASSERT_MSG_NOT_NULL);

	mCameras[static_cast<UINT>(CAMERA_PRIORITY_TYPE)] = camera;
}

void RenderTargetRenderer::PopUpCamera(const eCameraPriorityType priorityType)
{	
	Assert(mCameras[static_cast<UINT>(priorityType)], ASSERT_MSG_NULL);
	mCameras[static_cast<UINT>(priorityType)] = nullptr;
}

void RenderTargetRenderer::registerRenderComponent(RenderComponent* const renderComponent)
{
	Assert(renderComponent, ASSERT_MSG_NULL);

	const eRenderType RENDER_PRIORITY_TYPE = renderComponent->GetMaterial(0)->GetRenderType();

	if (eRenderType::PostProcess == RENDER_PRIORITY_TYPE)
	{
		mPostProcessComponents.push_back(renderComponent);
	}
	else
	{
		mRenderComponentsArray[static_cast<UINT>(RENDER_PRIORITY_TYPE)].push_back(renderComponent);
	}

}

void RenderTargetRenderer::registerLightInfo(const tLightInfo& light2DInfo)
{
	mLight2DInfos.push_back(light2DInfo);
}

void RenderTargetRenderer::zSortRenderObjectArray(const eRenderType renderPriorityType)
{
	auto& renderObjects = mRenderComponentsArray[static_cast<UINT>(renderPriorityType)];

	std::sort(renderObjects.begin(), renderObjects.end(), [](RenderComponent* const lhs, RenderComponent* const rhs)
		{
			const Transform* const lhsTransform = lhs->GetOwner()->GetComponent<Transform>();
			const Transform* const rhsTransform = rhs->GetOwner()->GetComponent<Transform>();
			return lhsTransform->GetPosition().z < rhsTransform->GetPosition().z;
		});
}

void RenderTargetRenderer::Render(const UINT renderTargetWidth,
	const UINT renderTargetHeight,
	ID3D11RenderTargetView** const ppRenderTargetView,
	ID3D11DepthStencilView* const depthStencilView) const
{
	//렌더 준비단계 
	gGraphicDevice->BindRenderTarget(renderTargetWidth,
		renderTargetHeight,
		1,
		ppRenderTargetView,
		depthStencilView);

	const void* const P_LIGHT_DATA = mLight2DInfos.data();
	if (nullptr != P_LIGHT_DATA)
	{
		gGraphicDevice->PassSB(eSBType::Light2D, sizeof(tLightInfo),
			static_cast<UINT>(mLight2DInfos.size()), P_LIGHT_DATA);

		gGraphicDevice->BindSB(eSBType::Light2D, eShaderBindType::PS);
	}

	tGlobalInfo globalInfo = {};

	globalInfo.Resolution = Vector2(static_cast<float>(renderTargetWidth), static_cast<float>(renderTargetHeight));
	globalInfo.Deltatime = gDeltaTime;
	globalInfo.Light2DCount = static_cast<UINT>(mLight2DInfos.size());
	globalInfo.GlobalTime = gGlobalTime;

	gGraphicDevice->PassCB(eCBType::GlobalInfo, sizeof(globalInfo), &globalInfo);
	gGraphicDevice->BindCB(eCBType::GlobalInfo, eShaderBindType::VS);
	gGraphicDevice->BindCB(eCBType::GlobalInfo, eShaderBindType::PS);

	//FIXME Graphic Debug
	gGraphicDevice->PassCB(eCBType::GraphicDebug, sizeof(tGraphicDebug), &mGraphicDebug);
	gGraphicDevice->BindCB(eCBType::GraphicDebug, eShaderBindType::VS);
	gGraphicDevice->BindCB(eCBType::GraphicDebug, eShaderBindType::PS);

	//Depth Shadow Map
	Texture* shadowMap = gResourceManager->Find<Texture>(L"ShadowMap_0");
	gGraphicDevice->BindSRV(eShaderBindType::PS, 8, shadowMap);
	(void)shadowMap;

	for (const Camera* const P_CAMERA : mCameras)
	{
		if (nullptr == P_CAMERA)
		{
			continue;
		}

		const UINT CAMERA_PRIORITY = static_cast<UINT>(P_CAMERA->GetPriorityType());
		if (!(mCameraMask & (1 << CAMERA_PRIORITY)))
		{
			continue;
		}

		const UINT LAYER_MASK = P_CAMERA->GetLayerMask();
		const UINT RENDER_MASK = P_CAMERA->GetRenderMask();

		for (UINT i = 0; i < static_cast<UINT>(eRenderType::End); ++i)
		{
			auto& renderComponents = mRenderComponentsArray[i];			
			setBindRenderTarget(static_cast<eRenderType>(i));

			for (RenderComponent* const renderComponent : renderComponents)
			{
				const UINT LAYER  = static_cast<UINT>(renderComponent->GetOwner()->GetLayer());
				const UINT RENDER = static_cast<UINT>(renderComponent->GetMaterial(0)->GetRenderType());

				//FIXME 그리기전에(register에서) 필터링하기
				if ((LAYER_MASK & (1 << LAYER)) && (RENDER_MASK & (1 << RENDER)))
				{
					renderComponent->render(P_CAMERA->GetView(), P_CAMERA->GetProjection());
				}
			}
		}
	}


	//PostProcess
	const Camera* const P_MAIN_CAMERA = mCameras[static_cast<UINT>(eCameraPriorityType::Main)];
	if (P_MAIN_CAMERA)
	{
		Texture* const copyTexture = gResourceManager->Find<Texture>(L"CopyRenderTargetTexture");

		//다음에 고치러 오시오
		//해상도마다 복사본 텍스처 사이즈가 달라짐
		//FIXME
		(void)copyTexture;
		for (RenderComponent* const postProcessComponent : mPostProcessComponents)
		{
			//FIXME참조카운팅
			ID3D11Resource* renderTargetTexture = nullptr;
			(*ppRenderTargetView)->GetResource(&renderTargetTexture);
			Assert(renderTargetTexture, ASSERT_MSG_NULL);

			gGraphicDevice->CopyResource(copyTexture->GetID3D11Texture2D(), renderTargetTexture);
			gGraphicDevice->BindSRV(eShaderBindType::PS, 10, copyTexture);
			postProcessComponent->render(P_MAIN_CAMERA->GetView(), P_MAIN_CAMERA->GetProjection());

			renderTargetTexture->Release();
			renderTargetTexture = nullptr;
		}		
	}

	const Camera* const P_EDITOR_CAMERA = mCameras[static_cast<UINT>(eCameraPriorityType::Editor)];
	if (mCameras[static_cast<UINT>(eCameraPriorityType::Editor)] && mbDebugRender)
	{
		mDebugRenderer->render(P_EDITOR_CAMERA);
	}
}

void RenderTargetRenderer::RenderShadowMap(Transform* transform, 
	const UINT renderTargetWidth,
	const UINT renderTargetHeight,
	ID3D11DepthStencilView* const depthStencilView) const
{
	gGraphicDevice->BindRenderTarget(renderTargetWidth,
		renderTargetHeight,
		0,
		nullptr,
		depthStencilView);

	Assert(transform, ASSERT_MSG_NULL);
	Assert(depthStencilView, ASSERT_MSG_NULL);
	(void)renderTargetHeight;
	(void)renderTargetWidth;

	//transform->CalculateTransform();

	Matrix viewMatrix = Camera::CreateViewMatrix(transform);
	Matrix projectionMatrix = Camera::CreateProjectionOrthographicMatrix(renderTargetWidth, renderTargetHeight, 1.f, 1.f, 10000.f);
	
	for (UINT i = 0; i < static_cast<UINT>(eRenderType::End); ++i)
	{
		auto& renderComponents = mRenderComponentsArray[i];
		//setBindRenderTarget(static_cast<eRenderType>(i));

		for (RenderComponent* const renderComponent : renderComponents)
		{
			//const UINT LAYER = static_cast<UINT>(renderComponent->GetOwner()->GetLayer());
			//const UINT RENDER = static_cast<UINT>(renderComponent->GetMaterial(0)->GetRenderType());

			//FIXME 그리기전에(register에서) 필터링하기
			renderComponent->render(viewMatrix, projectionMatrix);
		}
	}
}

void RenderTargetRenderer::flush()
{

	//TEST CODE
	//if (gInput->GetKeyDown(eKeyCode::L))
	//{
	//	if (IsVisibleDebugRenderer())
	//	{
	//		TurnOffDebugRenderer();
	//	}
	//	else
	//	{
	//		TurnOnDebugRenderer();
	//	}
	//}


	mDebugRenderer->flush();

	//TODO: 추후에 다시확인
	//for (auto& camera : mCameras)
	//{
	//	camera = nullptr;
	//}

	for (auto& renderComponents : mRenderComponentsArray)
	{
		renderComponents.clear();
	}

	mPostProcessComponents.clear();

	mLight2DInfos.clear();
}

void RenderTargetRenderer::setBindRenderTarget(const eRenderType type) const
{
	switch (type)
	{
	case eRenderType::Defereed:
		break;
	case eRenderType::Opqaue:
		break;
	case eRenderType::CutOut:
		break;
	case eRenderType::Transparent:
		break;
	case eRenderType::End:
		break;
	case eRenderType::PostProcess:
		break;
	default:
		break;
	}
}
