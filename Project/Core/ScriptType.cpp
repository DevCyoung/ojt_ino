#include "pch.h"
#include "ScriptType.h"
#include "ScriptComponent.h"

eScriptComponentType GetScriptComponentType(const ScriptComponent* const scriptComponent)
{
	return scriptComponent->GetScriptType();
}