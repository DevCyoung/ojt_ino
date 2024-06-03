#include "pch.h"
#include "Collider2D.h"
#include "Transform.h"
#include "RenderTargetRenderer.h"
#include "DebugRenderer2D.h"
#include "ScriptComponent.h"

Collider2D::Collider2D()
	:Component(eComponentType::Collider2D)
{

}

Collider2D::Collider2D(const eComponentType componentType,
	const eCollider2DType colllider2DType)
	: Component(componentType)
	, mColllider2DType(colllider2DType)
	, mColliderWorldMat{}
	, mSize{ 100.f, 100.f, 1.f }
	, mOffset{}
	, mCollisionCount(0)
	, mRadius(50.f)
	, mStartPos(Vector3(-100.f, 100.f, 0.f))
	, mEndPos(Vector3(100.f, -100.f, 0.f))
	, mbVisible(true)
	, mColor(0.f, 1.f, 0.f, 1.f)
{

}

Collider2D::~Collider2D()
{
}

Collider2D::Collider2D(const Collider2D& other)
	:Component(other)
{
}

void Collider2D::initialize()
{
}

void Collider2D::update()
{
}

void Collider2D::lateUpdate()
{

}

void Collider2D::onCollisionEnter(Collider2D* other)
{
	++mCollisionCount;

	const std::vector<ScriptComponent*>& scripts = GetOwner()->GetScriptComponents();

	for (ScriptComponent* script : scripts)
	{
		script->onCollisionEnter(other);
	}
}

void Collider2D::onCollisionStay(Collider2D* other)
{
	const std::vector<ScriptComponent*>& scripts = GetOwner()->GetScriptComponents();

	for (ScriptComponent* script : scripts)
	{
		script->onCollisionStay(other);
	}
}

void Collider2D::onCollisionExit(Collider2D* other)
{
	--mCollisionCount;

	const std::vector<ScriptComponent*>& scripts = GetOwner()->GetScriptComponents();

	for (ScriptComponent* script : scripts)
	{
		script->onCollisionExit(other);
	}
}
