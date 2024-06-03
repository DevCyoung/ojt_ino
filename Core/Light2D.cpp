#include "pch.h"
#include "Light2D.h"
#include "Transform.h"

#include "GameSystem.h"
#include "RenderTargetRenderer.h"

#include "EngineMath.h"

Light2D::Light2D()
	: Component(eComponentType::Light2D)
	, mSpotAngle(0.f)
	, mbOn(true)
{
}

Light2D::~Light2D()
{
}

void Light2D::initialize()
{
}

void Light2D::update()
{
}

void Light2D::lateUpdate()
{
	if (false == mbOn)
	{
		return;
	}

	Transform* const transform = GetOwner()->GetComponent<Transform>();

	const Vector3& WORLD_POS = transform->GetWorldMatrix().Translation();
	const Vector3& RIGHT_DIR = transform->GetRight();

	m_LightInfo.WorldPos = Vector4(WORLD_POS.x, WORLD_POS.y, WORLD_POS.z, 1.f);	
	m_LightInfo.WorldDir = Vector4(RIGHT_DIR.x, RIGHT_DIR.y, RIGHT_DIR.z, 1.f);
	m_LightInfo.Angle = Deg2Rad(mSpotAngle);
	
	GetOwner()->GetGameSystem()->GetRenderTargetRenderer()->registerLightInfo(m_LightInfo);
}
