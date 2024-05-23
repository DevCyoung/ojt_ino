#include "pch.h"
#include "CameraInputMoveMent.h"
#include "Components.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "MessageManager.h"

CameraInputMoveMent::CameraInputMoveMent()
	: ScriptComponent(eScriptComponentType::CameraInputMoveMent)
{
}



CameraInputMoveMent::~CameraInputMoveMent()
{
}

void CameraInputMoveMent::initialize()
{
}
void CameraInputMoveMent::MoveCamera()
{
	Camera* const camera = GetOwner()->GetComponent<Camera>();

	if (gInput->GetKeyDown(eKeyCode::F1))
	{
		camera->SetProjectionType(eCameraProjectionType::Orthographic);
	}
	else if (gInput->GetKeyDown(eKeyCode::F2))
	{
		camera->SetProjectionType(eCameraProjectionType::Perspective);
	}	

	constexpr float cameraSpeed = 600.f;

	float fSpeed = cameraSpeed;

	if (gInput->GetKey(eKeyCode::LSHIFT))
	{
		fSpeed *= 5.f;
	}

	Transform* const transform = GetOwner()->GetComponent<Transform>();
	if (camera->GetProjectionType() == eCameraProjectionType::Orthographic)
	{
		Vector3 pos = transform->GetPosition();
		Vector3 dir = Vector3::Zero;

		if (gInput->GetKey(eKeyCode::UP))
		{
			dir.y += 1.f;
		}
		if (gInput->GetKey(eKeyCode::DOWN))
		{
			dir.y += -1.f;
		}
		if (gInput->GetKey(eKeyCode::LEFT))
		{
			dir.x -= 1.f;
		}
		if (gInput->GetKey(eKeyCode::RIGHT))
		{
			dir.x += 1.f;
		}

		dir.Normalize();
		dir *= fSpeed * gDeltaTime;
		pos += dir;
		transform->SetPosition(pos);		

		float cameraSize = camera->GetCameraSize();

		if (gInput->GetKey(eKeyCode::NUM1))
		{
			cameraSize += gDeltaTime * 10.f;
		}
		if (gInput->GetKey(eKeyCode::NUM2))
		{
			cameraSize -= gDeltaTime * 10.f;
		}

		if (cameraSize < 0.01f)
		{
			cameraSize = 0.01f;
		}
		else if (cameraSize >= 5.f)
		{
			cameraSize = 5.f;
		}

		camera->Set2DSize(cameraSize);

		if (MessageManager::GetInstance()->IsAddTitleMessage())
		{
			wchar_t buffer[256] = { 0, };
			swprintf_s(buffer, 256, L"<Main Camera Position : %.2f, %.2f, %.2f Size : %.2f>", pos.x, pos.y, pos.z, cameraSize);
			MessageManager::GetInstance()->AddTitleMessage(buffer);
		}		
	}
	else
	{
		Vector3 pos = transform->GetPosition();
		Vector3 rot = transform->GetRotation();
		Vector3 forward = transform->GetForward();
		Vector3 right = transform->GetRight();

		if (gInput->GetKey(eKeyCode::W))
		{
			pos += gDeltaTime * forward * fSpeed;
		}

		if (gInput->GetKey(eKeyCode::S))
		{
			pos -= gDeltaTime * forward * fSpeed;
		}

		if (gInput->GetKey(eKeyCode::A))
		{
			pos -= gDeltaTime * right * fSpeed;
		}

		if (gInput->GetKey(eKeyCode::D))
		{
			pos += gDeltaTime * right * fSpeed;
		}

		//FIXME
		if (gInput->GetKey(eKeyCode::RBTN))
		{			
			Vector2 mouseDir = gInput->GetMouseDir();

			rot.y += gDeltaTime	* mouseDir.x * 100.f;
			rot.x -= gDeltaTime * mouseDir.y * 100.f;
		}

		if (gInput->GetKey(eKeyCode::UP))
		{
			pos += Vector3::Up * fSpeed * gDeltaTime;
		}

		if (gInput->GetKey(eKeyCode::DOWN))
		{
			pos += Vector3::Down * fSpeed * gDeltaTime;
		}

		transform->SetPosition(pos);
		transform->SetRotation(rot);
	}
}


void CameraInputMoveMent::update()
{
	MoveCamera();
}

void CameraInputMoveMent::lateUpdate()
{
}
