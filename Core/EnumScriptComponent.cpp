#include "pch.h"
#include "EnumScriptComponent.h"

//#include "CameraInputMoveMent.h"
//#include "CursorMovement.h"
//#include "PlayerMovement.h"
//#include "ThirdPersonOrbitCam.h"

static constexpr const wchar_t* const ScriptComponentNames[static_cast<UINT>(eScriptComponentType::End)]
{
	L"CameraInputMoveMent",
	L"CursorMovement",
	L"PlayerMovement",
	L"ThirdPersonOrbitCam",
};

const wchar_t* GetScriptComponentName(const eScriptComponentType type)
{
	assert(static_cast<UINT>(type) < static_cast<UINT>(eScriptComponentType::End));
	return ScriptComponentNames[static_cast<UINT>(type)];
}

ScriptComponent* CreateScriptComponentByName(const std::wstring& scriptcomponentName)
{
	ScriptComponent* scriptcomponent = nullptr;

	//if (L"CameraInputMoveMent" == scriptcomponentName)
	//	scriptcomponent = new CameraInputMoveMent;
	//else if (L"CursorMovement" == scriptcomponentName)
	//	scriptcomponent = new CursorMovement;
	//else if (L"PlayerMovement" == scriptcomponentName)
	//	scriptcomponent = new PlayerMovement;
	//else if (L"ThirdPersonOrbitCam" == scriptcomponentName)
	//	scriptcomponent = new ThirdPersonOrbitCam;
	//else
	//	assert(false);
	return scriptcomponent;
}

ScriptComponent* CreateScriptComponentByEnum(const eScriptComponentType type)
{
	ScriptComponent* scriptcomponent = nullptr;

	//switch (type)
	//{
	//case eScriptComponentType::CameraInputMoveMent:
	//	scriptcomponent = new CameraInputMoveMent;
	//	break;
	//case eScriptComponentType::CursorMovement:
	//	scriptcomponent = new CursorMovement;
	//	break;
	//case eScriptComponentType::PlayerMovement:
	//	scriptcomponent = new PlayerMovement;
	//	break;
	//case eScriptComponentType::ThirdPersonOrbitCam:
	//	scriptcomponent = new ThirdPersonOrbitCam;
	//	break;
	//default:
	//	assert(false);
	//	break;
	//}
	return scriptcomponent;
}

