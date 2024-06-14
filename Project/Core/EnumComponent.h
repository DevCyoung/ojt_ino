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
