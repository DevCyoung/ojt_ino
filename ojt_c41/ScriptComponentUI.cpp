# include "pch.h"
#include "ScriptComponent.h"
class Component;
void ComponentUI(Component* component);

void ScriptComponentUI(ScriptComponent* component)
{
	ComponentUI(component);
}
