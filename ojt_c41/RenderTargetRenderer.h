#pragma once
#include <d3d11.h>
#include "EnumRenderType.h"
#include "StructBuffer.h"

class Camera;
class RenderComponent;
class DebugRenderer2D;
class Light2D;
class Transform;
class RenderTargetRenderer
{
	friend class Camera;
	friend class RenderComponent;
	friend class Light2D;
	friend class Scene;

public:
	RenderTargetRenderer();
	virtual ~RenderTargetRenderer();

	void Render(const UINT renderTargetWidth,
		const UINT renderTargetHeight,
		ID3D11RenderTargetView** const ppRenderTargetView,
		ID3D11DepthStencilView* const depthStencilView) const;

	void RenderShadowMap(Transform* transform, 
		const UINT renderTargetWidth,
		const UINT renderTargetHeight,
		ID3D11DepthStencilView* const depthStencilView) const;


	Camera* GetRegisteredRenderCameraOrNull(const eCameraPriorityType priorityType) const
	{		
		return mCameras[static_cast<UINT>(priorityType)];
	}

	Camera* GetRegisteredRenderCamera(const eCameraPriorityType priorityType) const
	{		
		Assert(mCameras[static_cast<UINT>(priorityType)], ASSERT_MSG_NULL);

		return mCameras[static_cast<UINT>(priorityType)];
	}


	DebugRenderer2D* GetDebugRenderer2D() const { Assert(mDebugRenderer, ASSERT_MSG_NULL); return mDebugRenderer; }

	void SetCameraLayerMask(const UINT layerMask) { mCameraMask = layerMask; }
	UINT GetCameraLayerMask() const { return mCameraMask; }
	
	void TurnOnAllCamera()  { mCameraMask = 0XFFFFFFFF; }
	void TurnOffAllCamera() { mCameraMask = 0; }

	bool IsVisibleDebugRenderer() const { return mbDebugRender; }

	void TurnOnCamera(const eCameraPriorityType priorityType) 
	{
		mCameraMask |= (1 << static_cast<UINT>(priorityType));
	}

	void TurnOffCamera(const eCameraPriorityType priorityType)
	{
		mCameraMask &= ~(1 << static_cast<UINT>(priorityType));
	}

	void TurnOnDebugRenderer() { mbDebugRender = true; }
	void TurnOffDebugRenderer() { mbDebugRender = false; }

	void TurnOnWireFrame() { mbWireFrame = true; }
	void TurnOffWireFrame() { mbWireFrame = false; }
	bool IsWireFrame() { return mbWireFrame; }


	void RegisterRenderCamera(Camera* const camera);

	void PopUpCamera(const eCameraPriorityType priorityType);

	//FIXME
	void SetGraphicDebug(const tGraphicDebug& graphicDebug) { mGraphicDebug = graphicDebug; }


private:
	void registerRenderComponent(RenderComponent* const renderComponent);
	void registerLightInfo(const tLightInfo& light2DInfo);

	void zSortRenderObjectArray(const eRenderType priorityType);
	void flush();

	void setBindRenderTarget(const eRenderType type) const;



private:
	DebugRenderer2D* mDebugRenderer;
	Camera* mCameras[static_cast<UINT>(eCameraPriorityType::End)];
	std::vector<RenderComponent*> mRenderComponentsArray[static_cast<UINT>(eRenderType::End)];
	std::vector<RenderComponent*> mPostProcessComponents;
	std::vector<tLightInfo> mLight2DInfos;
	bool mbDebugRender;
	bool mbWireFrame;
	UINT mCameraMask;

	//FIXME
	tGraphicDebug mGraphicDebug;
};
