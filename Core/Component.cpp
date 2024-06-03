#include "pch.h"
#include "Component.h"
#include "GameObject.h"

Component::Component(const eComponentType componentType)
	: mType(componentType)
	, mOwner(nullptr)
{
}

Component::~Component()
{
}

Component::Component(const Component& other)
	: Entity(other)
	, mType(other.mType)
	, mOwner(nullptr)
{

}

Component* Component::GetComponentOrNull(const eComponentType componentType) const
{
	return GetOwner()->GetComponentOrNull(componentType);
}

ScriptComponent* Component::GetComponentOrNull(const eScriptComponentType scriptComponentType) const
{
	return GetOwner()->GetComponentOrNull(scriptComponentType);
}

Component* Component::GetComponent(const eComponentType componentType) const
{
	return GetOwner()->GetComponent(componentType);
}

ScriptComponent* Component::GetComponent(const eScriptComponentType scriptComponentType) const
{
	return GetOwner()->GetComponent(scriptComponentType);
}

void Component::initialize()
{
}

void Component::update()
{
}

void Component::fixedUpdate()
{
}

void Component::lateUpdate()
{
}

void Component::lastUpdate()
{
}

void Component::Save(FILE* const file)
{
	(void)file;
	//Save EntityName
}

void Component::Load(FILE* const file)
{
	(void)file;
}
