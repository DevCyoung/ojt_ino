# include "pch.h"
#include "Animator3D.h"
#include "SceneManager.h"
#include "GameSystem.h"
#include "RenderTargetRenderer.h"
#include "DebugRenderer2D.h"
#include "Transform.h"
#include "EditorSetting.h"
#include "BoneMap.h"
#include "Animation3DController.h"
class RenderComponent;
void ComponentUI(Component* component);


void Animator3DUI(Animator3D* component)
{
	ComponentUI(component);

	//const std::vector<tMTAnimClip>& clips =  *component->GetAnimationClip();
	static int item_current = 0;

	//std::vector<std::wstring> clipNames;
	//clipNames.reserve(clips.size());

	//for (const tMTAnimClip& clip : clips)
	//{		
	//	clipNames.push_back(clip.strAnimName);
	//}

	//if (ImGui::Combo("##Animation3DComboCLips", &item_current, clipNames))
	//{
	//	component->SetClipID(item_current);
	//}

	//Frame
	int curFrame = component->GetController()->GetCurFrameIdx();
	//ImGui::InputInt("cur frame : ", &curFrame);	


	int idx = item_current;
	if (idx < 0)
	{
		return;
	}
	//int startFrame = component->GetStartFrame(idx);
	//int endFrame = component->GetEndFrame(idx);
	//
	//if (ImGui::InputInt("starFrame", &startFrame))
	//{
	//	 component->SetStartFrame(idx, startFrame);
	//}
	//if (ImGui::InputInt("endFrame", &endFrame))
	//{
	//	component->SetEndFrame(idx, endFrame);
	//}		

	//Bone
	//Mesh
	const std::vector<tMTBone>& bones  = component->GetBones();	
	static int clickIdx = -1;
	if (ImGui::CollapsingHeader("BonData"))
	{
		for (int i = 0; i < bones.size(); ++i)
		{
			const tMTBone& bone = bones[i];
			std::string bonName = StringHelper::WStrToStr(bone.strBoneName);
			if (ImGui::Selectable(bonName.c_str()))
			{
				clickIdx = i;
			}
			if (bone.vecKeyFrame.empty())
			{
				continue;
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::Button("Add Bone"))
				{
					GameObject* obj =  EditorSetting::CreateObject();
					obj->AddComponent<BoneMap>();
					obj->GetComponent< BoneMap>()->SetAnimator3D(component, i);
					gCurrentScene->RegisterEventAddGameObject(obj, eLayerType::Default);
				}

				ImGui::EndPopup();
			}
			
			tMTKeyFrame key = bone.vecKeyFrame[curFrame];
			Matrix worldMatrix = component->GetCurGrameBoneMatrix(i);			
			Vector4 color = Vector4(0.f, 1.f, 0.f, 1.f);
			if (clickIdx != -1 && clickIdx == i)
			{
				color = Vector4(1.f, 0.f, 0.f, 1.f);
			}

			Matrix skeletonScaleMatrix = Matrix::CreateScale(EditorSetting::SkeletonScale, EditorSetting::SkeletonScale, EditorSetting::SkeletonScale);

			worldMatrix = skeletonScaleMatrix * worldMatrix;

			gCurrentSceneRenderer->GetDebugRenderer2D()->DrawCube3D(worldMatrix, 0, color);

			ImGui::InputFloat3("T", &key.vTranslate.x);
			ImGui::InputFloat3("S", &key.vScale.x);
			ImGui::InputFloat4("Q", &key.qRot.x);

			
		}
	}
}
