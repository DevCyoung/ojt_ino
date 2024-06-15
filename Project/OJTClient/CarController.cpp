#include "pch.h"
#include "CarController.h"
#include <Transform.h>
#include <TimeManager.h>
#include <InputManager.h>
#include "InnoDataManager.h"
#include <RenderTargetRenderer.h>
#include <DebugRenderer2D.h>
#include <SceneManager.h>
#include <GameSystem.h>
CarController::CarController()
	: ScriptComponent(eScriptComponentType::CarController)
	, mWheels()
	, mCamera(nullptr)
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
	Vector3 position = GetComponent<Transform>()->GetPosition();

	//포지션업데이트
	//if (InnoSimulator::GetInstance()->IsPlaying())
	//{		
	//	position.x = InnoDataManager::GetInstance()->GetCurrentSampleData().xPos;
	//	//position.x += InnoSimulator::GetInstance()->GetSpeed() * 100 * gDeltaTime;
	//	GetComponent<Transform>()->SetPosition(position);
	//}

	Vector3 debugPos = position;
	debugPos.y -= 80.f;
	gCurrentSceneRenderer->GetDebugRenderer2D()->DrawFillRect2D(Vector3(0.f, -300.f, 0.f), Vector2(10.f, 300.f), 0.f, Vector4(1.f, 0.f, 0.f, 1.f));
	gCurrentSceneRenderer->GetDebugRenderer2D()->DrawFillRect2D(Vector3(1000.f, -300.f, 0.f), Vector2(10.f, 300.f), 0.f, Vector4(0.f, 1.f, 0.f, 1.f));
	gCurrentSceneRenderer->GetDebugRenderer2D()->DrawFillRect2D(debugPos, Vector2(8.f, 20.f), 0.f, Vector4(1.f, 1.f, 1.f, 1.f));
	
	//Network Test
	{
		Vector3 pA = position;
		Vector3 pB = position;
		pB.y -= 200.f;

		if (!InnoDataManager::GetInstance()->GetTimes().empty())
		{
			pA.x = InnoDataManager::GetInstance()->GetXPoses().back();
			pB.x = InnoDataManager::GetInstance()->GetXOtherPoses().back();

			gCurrentSceneRenderer->GetDebugRenderer2D()->DrawFillRect2D(pA, Vector2(50.f, 50.f), 0.f, Vector4(1.f, 0.f, 0.f, 1.f));
			gCurrentSceneRenderer->GetDebugRenderer2D()->DrawFillRect2D(pB, Vector2(50.f, 50.f), 0.f, Vector4(0.f, 0.f, 1.f, 1.f));
		}		
	}

	for (int i = 0; i < mWheels.size(); ++i)
	{
		Vector3 rotation = mWheels[i]->GetComponent<Transform>()->GetRotation();
		rotation.z += 360 * gDeltaTime * 0.1f;
		mWheels[i]->GetComponent<Transform>()->SetRotation(rotation);
	}

	//offset
	float xOffset = 450.f;
	float yOffset = 120.f;

	position.x += xOffset;
	position.y += yOffset;
	if (mCamera)
	{
		mCamera->GetComponent<Transform>()->SetPosition(position);

	}
}

void CarController::lateUpdate()
{
}
