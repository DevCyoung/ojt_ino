#include "pch.h"
#include "CarController.h"
#include <Transform.h>
#include <TimeManager.h>
#include <InputManager.h>

CarController::CarController()
	: ScriptComponent(eScriptComponentType::CarController)
	, mWheels()
{
}

CarController::~CarController()
{
}

void CarController::initialize()
{
}

void CarController::update()
{

	

	if (gInput->GetKey(eKeyCode::A))
	{
		Vector3 position = GetComponent<Transform>()->GetPosition();
		position.x -= gRealDeltaTime * 70.f;
		GetComponent<Transform>()->SetPosition(position);
	}

	for (int i = 0; i < mWheels.size(); ++i)
	{
		Vector3 rotation = mWheels[i]->GetComponent<Transform>()->GetRotation();
		rotation.z += 360 * gDeltaTime * 0.1f;
		mWheels[i]->GetComponent<Transform>()->SetRotation(rotation);
	}
}

void CarController::lateUpdate()
{
}
