#include "pch.h"
#include "ScriptComponent.h"
#include "Transform.h"

ScriptComponent::ScriptComponent(const eScriptComponentType scriptComonentType)
	: Component(eComponentType::ScriptComponent)
	, mScriptComponentType(scriptComonentType)	
{
}

ScriptComponent::~ScriptComponent()
{
}

ScriptComponent::ScriptComponent(const ScriptComponent& other)
	: Component(other)
	, mScriptComponentType(other.mScriptComponentType)
{
}

void ScriptComponent::initialize()
{
}

void ScriptComponent::update()
{
}

void ScriptComponent::fixedUpdate()
{

}

void ScriptComponent::lateUpdate()
{
}

void ScriptComponent::lastUpdate()
{
}

void ScriptComponent::onCollisionEnter(Collider2D* const other)
{
	UNREFERENCED_PARAMETER(other);
}

void ScriptComponent::onCollisionStay(Collider2D* const other)
{
	UNREFERENCED_PARAMETER(other);
}

void ScriptComponent::onCollisionExit(Collider2D* const other)
{
	UNREFERENCED_PARAMETER(other);
}
