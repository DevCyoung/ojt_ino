#include "pch.h"
#include "FolderViewUI.h"
#include "PathManager.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "EnumResource.h"
#include "Material.h"
#include "FBXLoader.h"
#include "MeshData.h"
#include "SceneManager.h"
#include "EnumLayer.h"
#include "GameObject.h"
#include "Builder.h"
//#include "InspectorUI.h"
#include "PanelUIManager.h"
#include <Shlwapi.h> // PathFindFileName 함수가 정의된 헤더 파일

#pragma comment(lib, "Shlwapi.lib") // Shlwapi 라이브러리 링크
namespace fs = std::filesystem;

std::wstring GetFileNameWithoutExtension(const std::wstring& filePath) {
	wchar_t fileName[MAX_PATH];
	wcscpy_s(fileName, MAX_PATH, PathFindFileName(filePath.c_str()));
	PathRemoveExtension(fileName);
	return fileName;
}


static std::vector<std::wstring> GetDirectoryNames(const std::wstring& path)
{
	std::vector<std::wstring> result;
	result.reserve(10);
	for (const auto& entry : fs::directory_iterator(path))
	{
		std::wstring path2 = entry.path();
		if (entry.is_directory())
		{
			result.push_back(path2);
		}
	}
	return result;
}


static eResourceType GetResTypeByExt(const std::wstring& _relativepath)
{
	wchar_t szExt[50] = {};
	_wsplitpath_s(_relativepath.c_str(), 0, 0, 0, 0, 0, 0, szExt, 50);
	std::wstring strExt = szExt;

	if (L".pf" == strExt)
		return eResourceType::Prefab;
	else if (L".mesh" == strExt)
		return eResourceType::Mesh;
	else if (L".mat" == strExt)
		return eResourceType::Material;
	else if (L".png" == strExt || L".jpg" == strExt
		|| L".jpeg" == strExt || L".bmp" == strExt
		|| L".tga" == strExt || L".dds" == strExt)
		return eResourceType::Texture;
	else if (L".mp3" == strExt || L".wav" == strExt || L".oga" == strExt)
		return eResourceType::Sound;
	else if (L".mesh_data" == strExt)
		return eResourceType::MeshData;
	else if (L".anim" == strExt)
		return eResourceType::Animation;
	else if (L".font" == strExt)
		return eResourceType::Font;
	else if (L".scene" == strExt)
		return eResourceType::Scene;
	else if (L".hlsl" == strExt || L".fxh" == strExt)
		return eResourceType::Shader;
	else if (L".fbx" == strExt || L".FBX" == strExt)
		return eResourceType::FBX;
	else
		return eResourceType::End;
}

static std::vector<std::wstring> GetFileNames(const std::wstring& path)
{
	std::vector<std::wstring> result;

	result.reserve(10);
	for (const auto& entry : fs::directory_iterator(path))
	{
		std::wstring path2 = entry.path();
		if (entry.is_regular_file())
		{
			result.push_back(path2);
		}
	}

	return result;
}

const Texture* GetFileUITexture(eResourceType resType)
{
#pragma region UI_TEXTURE


	static const Texture* const meshTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");

	static const Texture* const shaderTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\HlslUI.png");

	static const Texture* const animationTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");

	static const Texture* const fontTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");

	static const Texture* const soundTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");

	static const Texture* const sceneTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");

	static const Texture* const preFabTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");

	static const Texture* const computeShdaerTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");

	static const Texture* const audioClipTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");

	static const Texture* const dummyTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");

	static const Texture* const  materialTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");

	static const Texture* const meshDataTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");

	static const Texture* const fbxDataTex =
		gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\MaterialUI.jpg");
#pragma endregion
	const Texture* fileUI = dummyTex;
	switch (resType)
	{
	/*case eResourceType::Texture:
		break;*/
	case eResourceType::Mesh:
		fileUI = meshTex;
		break;
	case eResourceType::Material:
		fileUI = materialTex;
		break;
	case eResourceType::Shader:
		fileUI = shaderTex;
		break;
	case eResourceType::Animation:
		fileUI = animationTex;
		break;
	case eResourceType::Font:
		fileUI = fontTex;
		break;
	case eResourceType::Sound:
		fileUI = soundTex;
		break;
	case eResourceType::Scene:
		fileUI = sceneTex;
		break;
	case eResourceType::Prefab:
		fileUI = preFabTex;
		break;
	case eResourceType::ComputeShader:
		fileUI = computeShdaerTex;
		break;
	case eResourceType::AudioClip:
		fileUI = audioClipTex;
		break;
	case eResourceType::MeshData:
		fileUI = meshDataTex;
		break;
	case eResourceType::FBX:
		fileUI = fbxDataTex;
		break;
	case eResourceType::End:
		break;
	default:
		break;
	}
	return fileUI;
}
FolderViewUI::FolderViewUI()
	: mCD()
{
	SetTitle("FolderViewUI");
	mCD.push(gPathManager->GetResourcePath());
}

FolderViewUI::~FolderViewUI()
{
}

void FolderViewUI::Pop()
{
	if (mCD.size() <= 1)
	{
		return;
	}

	mCD.pop();
}

void FolderViewUI::Enter(const std::string& enter)
{
	(void)enter;
}

void FolderViewUI::update()
{
}


void FolderViewUI::drawForm()
{
	static float wrap_width = 70.f;
	static float icon_width = 70.0f;
	static const Texture* const folderTex = gResourceManager->Find<Texture>(L"\\Texture\\ImGUI\\folder.png");
	ImVec2 renderTargetSize = ImVec2(icon_width, icon_width);

	ImGui::Begin("FolderViewUI");
	ImGui::Text(StringHelper::WStrToStr(mCD.top()).c_str());
	if (ImGui::Button("Revert"))
	{
		if (mCD.size() > 1)
		{
			mCD.pop();
		}
	}
	ImGui::Separator();

	const std::wstring& curPath = mCD.top();
	std::vector<std::wstring> directorys = GetDirectoryNames(curPath);
	const std::wstring& resourcePath = gPathManager->GetResourcePath();

	//static InspectorUI* inspectorUI =
	//	static_cast<InspectorUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("InspectorUI"));



	int idx = 0;
	ImGui::SliderFloat("Icon width##FolderViewUI", &icon_width, -20, 100, "%.0f");
	ImGui::SliderFloat("Wrap width##FolderViewUI", &wrap_width, -20, 100, "%.0f");
	

	for (std::wstring& directory : directorys)
	{
		ImGui::BeginGroup();
		std::string directoryID = "##";
		directoryID += StringHelper::WStrToStr(directory);
		if (ImGui::ImageButton(directoryID.c_str(), folderTex->GetSRV(), renderTargetSize))
		{
			mCD.push(directory);
		}

		std::wstring name = StringHelper::GetDirectoryNameFromPath(directory);
		std::string strName = StringHelper::WStrToStr(name);
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
		ImGui::Text(strName.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndGroup();
		if (idx == 0 || idx % 11 != 0)
		{
			ImGui::SameLine();
		}
		else
		{
			idx = -1;
		}
		++idx;
	}
	static ImGuiTextFilter filter;

	std::vector<std::wstring> filePaths = GetFileNames(curPath);

	for (std::wstring filePath : filePaths)
	{
		std::wstring name = GetFileNameWithoutExtension(filePath);
		std::string strName = StringHelper::WStrToStr(name);
		eResourceType type = GetResTypeByExt(filePath);
		std::wstring relativePath = filePath.substr(resourcePath.size());
		ImGui::BeginGroup();

		const Texture* resFileUI = GetFileUITexture(type);

		switch (type)
		{
		case eResourceType::Texture:
		{
			Texture* tex = gResourceManager->Find<Texture>(relativePath);
			if (ImGui::ImageButton(strName.c_str(), tex->GetSRV(), renderTargetSize))
			{
				//inspectorUI->Register(tex);
			}
		}

		break;
		case eResourceType::Mesh:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				//Mesh* mat = gResourceManager->Find<Mesh>(relativePath);
				//inspectorUI->Register(mat);
			}
		}
		break;
		break;
		case eResourceType::Material:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				Material* mat = gResourceManager->Find<Material>(relativePath);
				//inspectorUI->Register(mat);
			}
		}
		break;
		case eResourceType::Shader:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				//Shader* shader = gResourceManager->Find<Shader>(relativePath);
				//inspectorUI->Register(shader);
			}
		}
		break;
		case eResourceType::Animation:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				//Animation* anim = gResourceManager->Find<Animation>(relativePath);
				//inspectorUI->Register(anim);
			}
		}
		break;
		case eResourceType::Font:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				//Font* font = gResourceManager->Find<Font>(relativePath);
				//inspectorUI->Register(font);
			}
		}
		break;
		case eResourceType::Sound:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				//Sound* sound = gResourceManager->Find<Sound>(relativePath);
				//inspectorUI->Register(sound);
			}
		}
		break;
		case eResourceType::Scene:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				//Scene* scene = gResourceManager->Find<Scene>(relativePath);
				//inspectorUI->Register(scene);
			}
		}
		break;
		case eResourceType::Prefab:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				//Prefab* prefab = gResourceManager->Find<Prefab>(relativePath);
				//inspectorUI->Register(prefab);
			}

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::Button("Instantiate"))
				{
					gPathManager->GetResourcePath();
					FILE* file = nullptr;
					_wfopen_s(&file, filePath.c_str(), L"rb");
					GameObject* obj = CreateGameObjectNoTransform();
					obj->Load(file);
					gCurrentScene->RegisterEventAddGameObject(obj, obj->GetLayer());
					fclose(file);
				}
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::SetItemTooltip("Right-click to open popup");

		}
		break;
		case eResourceType::ComputeShader:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				//ComputeShader* csahder = gResourceManager->Find<ComputeShader>(relativePath);
				//inspectorUI->Register(csahder);
			}
		}
		break;
		case eResourceType::AudioClip:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				//AudioClip* audioClip = gResourceManager->Find<AudioClip>(relativePath);
				//inspectorUI->Register(audioClip);
			}
		}
		break;
		case eResourceType::MeshData:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				//MeshData* meshData = gResourceManager->Find<MeshData>(relativePath);
				//inspectorUI->Register(meshData);
			}

			MeshData* meshData = gResourceManager->FindAndLoadOrNull<MeshData>(relativePath);

 			if (meshData && ImGui::BeginPopupContextItem())
			{
				if (ImGui::Button("Instantiate"))
				{					
					GameObject* obj = meshData->Instantiate();
					obj->SetName(name);
					SceneManager::GetInstance()->GetCurrentScene()->
						RegisterEventAddGameObject(obj, eLayerType::Default);
					
				}
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
		}
		break;
		case eResourceType::FBX:
		{
			if (ImGui::ImageButton(strName.c_str(), resFileUI->GetSRV(), renderTargetSize))
			{
				//MeshData* meshData = gResourceManager->Find<MeshData>(relativePath);
				//inspectorUI->Register(meshData);
			}
			//gResourceManager->Find<Texture>(relativePath)
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::Button("Load"))
				{
					MeshData* meshData = FBXLoader::FbxInstantiate(relativePath);					
					meshData->Save(L"\\" + GetFileNameWithoutExtension(relativePath));
					delete meshData;
					//gResourceManager->Find<Texture>(relativePath)
				}					
				if (ImGui::Button("Close"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			ImGui::SetItemTooltip("Right-click to open popup");
		}
		break;
		default:
			break;
		}



		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
		ImGui::Text(strName.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndGroup();
		if (idx == 0 || idx % 11 != 0)
		{
			ImGui::SameLine();
		}
		else
		{
			idx = -1;
		}
		++idx;
	}
	ImGui::End();
}