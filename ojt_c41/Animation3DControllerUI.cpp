# include "pch.h"
#include "Animation3DController.h"
#include "Animator3D.h"
class Component;
void ComponentUI(Component* component);


void Animation3DControllerUI(Animation3DController* component)
{
	ComponentUI(component);

	const std::vector<Animator3D*>& animation3Ds = component->GetAnimation3Ds();

	bool bStop = component->IsStop();

	if (ImGui::Checkbox("stop", &bStop))
	{
		component->Stop(bStop);
	}

	if (!animation3Ds.empty())
	{		
		int curFrame = component->GetCurFrameIdx();
		ImGui::Text("frame %d", curFrame);		

		std::vector<tMTAnimClip>& clips = component->GetAnimationClip();
		static int item_current = 0;

		std::vector<std::wstring> clipNames;
		clipNames.reserve(clips.size());

		for (const tMTAnimClip& clip : clips)
		{
			clipNames.push_back(clip.strAnimName);
		}

		if (ImGui::Combo("##Animation3DComboCLips", &item_current, clipNames))
		{
			//component->SetClipID(item_current);		
			component->SetClip(item_current);
		}

		int idx = item_current;

		if (idx < 0)
		{
			return;
		}

		int startFrame = clips[idx].iStartFrame;
		int endFrame = clips[idx].iEndFrame;

		if (ImGui::InputInt("starFrame : %d", &startFrame))
		{
			clips[idx].iStartFrame = startFrame;
		}

		if (ImGui::InputInt("endFrame : %d", &endFrame))
		{
			clips[idx].iEndFrame = endFrame;
		}

		if (bStop && ImGui::InputInt("stop##frame", &curFrame))
		{
			if (curFrame > endFrame)
			{
				curFrame = startFrame;
			}

			if (curFrame < startFrame)
			{
				curFrame = endFrame;
			}
			component->SetCurFrameIde(curFrame);
		}

		static int createStartFrame = 0;
		static int createEndFrame   = 0;

		ImGui::Text("Create Clip");
		static char buf[256] = {0,};
		ImGui::InputText("##Create Clip", buf, 256);

		if (ImGui::InputInt("starFrame", &createStartFrame))
		{
			//component->SetStartFrame(idx, startFrame);
		}
		if (ImGui::InputInt("endFrame", &createEndFrame))
		{
			//component->SetEndFrame(idx, endFrame);
		}

		bool flag = true;
		const std::wstring& bufStr = StringHelper::StrToWStr(buf);
		if (ImGui::Button("Create Clip"))
		{
			std::vector<tMTAnimClip>::iterator iter = clips.begin();
			
			for (; iter != clips.end(); ++iter)
			{
				if (iter->strAnimName == bufStr)
				{
					flag = false;
					break;
				}
			}

			if (flag)
			{
				tMTAnimClip clip = {};
				clip.iStartFrame = createStartFrame;
				clip.iEndFrame = createEndFrame;
				clip.strAnimName = StringHelper::StrToWStr(buf);
				clips.push_back(clip);
				component->SetAnimClip(clips);
			}
		}
		if (clips.size() > 1 && ImGui::Button("Remove Clip"))
		{			
			component->RemoveClip(clipNames[idx]);
		}
	}
}
