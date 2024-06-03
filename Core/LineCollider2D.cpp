#include "pch.h"
#include "Transform.h"
#include "LineCollider2D.h"
#include "RenderTargetRenderer.h"
#include "DebugRenderer2D.h"
#include "GameSystem.h"

LineCollider2D::LineCollider2D()
	: Collider2D(eComponentType::LineCollider2D, eCollider2DType::Line)
{
}

LineCollider2D::~LineCollider2D()
{
}

void LineCollider2D::initialize()
{
}

void LineCollider2D::update()
{
}

void LineCollider2D::lateUpdate()
{
	const Transform* const P_TRANSFORM = GetOwner()->GetComponent<Transform>();

	mColliderWorldMat = P_TRANSFORM->GetWorldMatrix();

	Vector3 startPos = mStartPos +  mColliderWorldMat.Translation();
	startPos.z = 0.f;
	Vector3 endPos   = mEndPos + mColliderWorldMat.Translation();
	endPos.z = 0.f;

	if (mbVisible)
	{
		DebugRenderer2D* debugRenderer = GetOwner()->GetGameSystem()->GetRenderTargetRenderer()->GetDebugRenderer2D();

		Vector4 color = Vector4(0.f, 1.f, 0.f, 1.f);

		if (GetOwner()->GetLayer() == eLayerType::LeftSlope)
		{
			color = Vector4(1.f, 1.f, 0.f, 1.f);
		}
		if (GetOwner()->GetLayer() == eLayerType::RightSlope)
		{
			color = Vector4(1.f, 0.5f, 0.5f, 1.f);
		}
		else if (GetOwner()->GetLayer() == eLayerType::Platform)
		{
			color = Vector4(1.f, 0.f, 1.f, 1.f);
		}
		else
		{
			color = mColor;
		}


		if (mCollisionCount > 0)
		{
			color = Vector4(1.f, 0.f, 0.f, 1.f);
		}

		debugRenderer->DrawLine2D(startPos, endPos, 0.f, color);
	}	
}
