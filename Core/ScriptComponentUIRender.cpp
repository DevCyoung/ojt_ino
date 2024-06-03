#include "pch.h"
#include "ComponentUIRender.h"
#include "ScriptComponent.h"
#include "EnumComponent.h"
#include "EnumScriptComponent.h"
class ScriptComponent;
void ScriptComponentUI(ScriptComponent* component);
class CameraInputMoveMent;
static void CameraInputMoveMentUI(CameraInputMoveMent* component) {};
class CursorMovement;
static void CursorMovementUI(CursorMovement* component) {};
class PlayerMovement;
static void PlayerMovementUI(PlayerMovement* component) {};
class ThirdPersonOrbitCam;
static void ThirdPersonOrbitCamUI(ThirdPersonOrbitCam* component) {};

void ScriptComponentUIRender(ScriptComponent* component)
{
	switch (component->GetScriptType())
	{
	case eScriptComponentType::CameraInputMoveMent:
		CameraInputMoveMentUI((CameraInputMoveMent*)component);
		break;
	case eScriptComponentType::CursorMovement:
		CursorMovementUI((CursorMovement*)component);
		break;
	case eScriptComponentType::PlayerMovement:
		PlayerMovementUI((PlayerMovement*)component);
		break;
	case eScriptComponentType::ThirdPersonOrbitCam:
		ThirdPersonOrbitCamUI((ThirdPersonOrbitCam*)component);
		break;
	}
}

