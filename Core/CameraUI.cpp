# include "pch.h"
#include "Camera.h"
class Component;
void ComponentUI(Component* component);


void CameraUI(Camera* component)
{
	ComponentUI(component);
	//component->get
	//eCameraProjectionType type = component->GetProjectionType();
	float fov = component->GetFOV();
	float _far = component->GetFar();
	float _near = component->GetNear();
	float ratio = component->GetAspectRatio();
	int mask = component->GetLayerMask();

	(void)mask;
	(void)ratio;

	ImGui::DragFloat("Fov", &fov);
	ImGui::DragFloat("Far", &_far);
	ImGui::DragFloat("Near", &_near);
	//ImGui::DragFloat("Ratio", &ratio);
	//ImGui::DragFloat("Mask", &_far);

	if (fov <= 0)
	{
		fov = 1.f;
	}

	if (_far <= 10)
	{
		_far = 10;
	}

	component->SetFOV(fov);
	component->SetFar(_far);
	component->SetNear(_near);
	//component->Setr(fov);

}
