#pragma once
#include <string>

class Component;

enum class eComponentType 
{
	Transform,
	Animation3DController,
	Animator2D,
	Animator3D,
	AudioListener,
	AudioSource,
	BoneMap,
	Camera,
	CircleCollider2D,
	Collider2D,
	Light2D,
	Light3D,
	LineCollider2D,
	MeshRenderer,
	ParticleSystem,
	RectCollider2D,
	RenderComponent,
	Rigidbody2D,
	SpriteRenderer,
	End,
	ScriptComponent
};

const wchar_t* GetComponentName(const eComponentType type);
Component* CreateComponentByName(const std::wstring& componentName);
Component * CreateComponentByEnum(const eComponentType type);

