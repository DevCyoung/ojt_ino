#pragma once
#include <string>

class ScriptComponent;

enum class eScriptComponentType 
{
	CameraInputMoveMent,
	CursorMovement,
	PlayerMovement,
	ThirdPersonOrbitCam,
	End
};

const wchar_t* GetScriptComponentName(const eScriptComponentType type);
ScriptComponent* CreateScriptComponentByName(const std::wstring& scriptcomponentName);
ScriptComponent * CreateScriptComponentByEnum(const eScriptComponentType type);

