#include "pch.h"
#include "ComponentUIRender.h"
#include "ScriptComponent.h"
#include "EnumComponent.h"
#include "EnumScriptComponent.h"

class Component;
void ComponentUI(Component* component);
class Animation3DController;
void Animation3DControllerUI(Animation3DController* component);
class Animator3D;
void Animator3DUI(Animator3D* component);
class AudioListener;
static void AudioListenerUI(AudioListener* component) {};
class AudioSource;
static void AudioSourceUI(AudioSource* component) {};
class BoneMap;
void BoneMapUI(BoneMap* component);
class Camera;
void CameraUI(Camera* component);
class Collider2D;
void Collider2DUI(Collider2D* component);
class CircleCollider2D;
void CircleCollider2DUI(CircleCollider2D* component);
class LineCollider2D;
void LineCollider2DUI(LineCollider2D* component);
class RectCollider2D;
void RectCollider2DUI(RectCollider2D* component);
class Light2D;
void Light2DUI(Light2D* component);
class Light3D;
void Light3DUI(Light3D* component);
class RenderComponent;
void RenderComponentUI(RenderComponent* component);
class Animator2D;
void Animator2DUI(Animator2D* component);
class MeshRenderer;
void MeshRendererUI(MeshRenderer* component);
class ParticleSystem;
void ParticleSystemUI(ParticleSystem* component);
class SpriteRenderer;
void SpriteRendererUI(SpriteRenderer* component);
class Rigidbody2D;
void Rigidbody2DUI(Rigidbody2D* component);
class ScriptComponent;
void ScriptComponentUI(ScriptComponent* component);
class Transform;
void TransformUI(Transform* component);

void ComponentUIRender(Component* component)
{
	switch (component->GetType())
	{
	case eComponentType::Animation3DController:
		Animation3DControllerUI((Animation3DController*)component);
		break;
	case eComponentType::Animator3D:
		Animator3DUI((Animator3D*)component);
		break;
	case eComponentType::AudioListener:
		AudioListenerUI((AudioListener*)component);
		break;
	case eComponentType::AudioSource:
		AudioSourceUI((AudioSource*)component);
		break;
	case eComponentType::BoneMap:
		BoneMapUI((BoneMap*)component);
		break;
	case eComponentType::Camera:
		CameraUI((Camera*)component);
		break;
	case eComponentType::Collider2D:
		Collider2DUI((Collider2D*)component);
		break;
	case eComponentType::CircleCollider2D:
		CircleCollider2DUI((CircleCollider2D*)component);
		break;
	case eComponentType::LineCollider2D:
		LineCollider2DUI((LineCollider2D*)component);
		break;
	case eComponentType::RectCollider2D:
		RectCollider2DUI((RectCollider2D*)component);
		break;
	case eComponentType::Light2D:
		Light2DUI((Light2D*)component);
		break;
	case eComponentType::Light3D:
		Light3DUI((Light3D*)component);
		break;
	case eComponentType::RenderComponent:
		RenderComponentUI((RenderComponent*)component);
		break;
	case eComponentType::Animator2D:
		Animator2DUI((Animator2D*)component);
		break;
	case eComponentType::MeshRenderer:
		MeshRendererUI((MeshRenderer*)component);
		break;
	case eComponentType::ParticleSystem:
		ParticleSystemUI((ParticleSystem*)component);
		break;
	case eComponentType::SpriteRenderer:
		SpriteRendererUI((SpriteRenderer*)component);
		break;
	case eComponentType::Rigidbody2D:
		Rigidbody2DUI((Rigidbody2D*)component);
		break;
	case eComponentType::ScriptComponent:
		ScriptComponentUI((ScriptComponent*)component);
		break;
	case eComponentType::Transform:
		TransformUI((Transform*)component);
		break;
	}
}

