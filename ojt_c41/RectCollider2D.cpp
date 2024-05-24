#include "pch.h"
#include "RectCollider2D.h"
#include "Transform.h"
#include "RenderTargetRenderer.h"
#include "DebugRenderer2D.h"
#include "GameSystem.h"
RectCollider2D::RectCollider2D()
	: Collider2D(eComponentType::RectCollider2D, eCollider2DType::Box)
{
}

RectCollider2D::~RectCollider2D()
{
}

void RectCollider2D::initialize()
{
}

void RectCollider2D::update()
{		
}

void RectCollider2D::lateUpdate()
{
	const Transform* const P_TRANSFORM = GetOwner()->GetComponent<Transform>();

	mColliderWorldMat = XMMatrixScaling(mSize.x, mSize.y, mSize.z);
	mColliderWorldMat *= XMMatrixTranslation(mOffset.x, mOffset.y, mOffset.z);
	mColliderWorldMat *= P_TRANSFORM->GetWorldMatrix();

	if (mbVisible)
	{
		DebugRenderer2D* debugRenderer = GetOwner()->GetGameSystem()->GetRenderTargetRenderer()->GetDebugRenderer2D();

		Vector4 color = Vector4(0.f, 1.f, 0.f, 1.f);

		if (mCollisionCount > 0)
		{
			color = Vector4(1.f, 0.f, 0.f, 1.f);
		}

		if (eLayerType::CameraWall == GetOwner()->GetLayer())
		{
			debugRenderer->DrawFillRect2D3(mColliderWorldMat, 0.f, Vector4(0.0f, 1.0f, 0.5f, 0.8f));
		}
		else
		{
			debugRenderer->DrawRect2D3(mColliderWorldMat, 0.f, color);
		}
	}	
}
