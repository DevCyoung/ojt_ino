#include "pch.h"
#include "EditorSetting.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "GameSystem.h"
#include "RenderTargetRenderer.h"
#include "ResourceManager.h"
#include "Components.h"
#include "Mesh.h"
#include "Material.h"
#include "Builder.h"
#include "PanelUIManager.h"
#include "EngineFile.h"

float EditorSetting::SkeletonScale = 1.f;

EditorSetting::EditorSetting()	
{
	SetTitle("Editor Setting");
}

EditorSetting::~EditorSetting()
{
}

GameObject* EditorSetting::CreateObject()
{
	GameObject* obj = CreateGameObject();
	obj->AddComponent<MeshRenderer>();
	obj->GetComponent<MeshRenderer>()->SetMesh(gResourceManager->Find<Mesh>(L"Cube"));
	obj->GetComponent<MeshRenderer>()->SetMaterial(gResourceManager->Find<Material>(L"Std3DMat"), 0);
	return obj;
}

void EditorSetting::drawForm()
{
	static bool bStartGame = true;

	ImGui::Begin("Editor Setting");		

	if (ImGui::Button("test dialog"))
	{
		helper::DialogPath dialogpath =  helper::SaveDialog();
		mCarSumulation.SaveData(dialogpath.path.c_str());

	}

	if (ImGui::Button("Sim Test"))
	{
		for (int i = 0; i < 3000; ++i)
		{
			mCarSumulation.Update();
		}		

		mCarSumulation.GetCS();
	}

	bool bWireFrame = gCurrentSceneRenderer->IsWireFrame();
	if (ImGui::Checkbox("Wire Frame", &bWireFrame))
	{
		if (bWireFrame)
		{
			gCurrentSceneRenderer->TurnOnWireFrame();
		}
		else
		{
			gCurrentSceneRenderer->TurnOffWireFrame();
		}
	}

	if (ImGui::Button("Create Object"))
	{
		GameObject* obj = CreateObject();
		gCurrentScene->RegisterEventAddGameObject(obj, eLayerType::Default);
	}

	//float mSkeletonScale = 1.f;

	if (ImGui::InputFloat("skeletonScale", &SkeletonScale, 0.01f, 5.f))
	{
	}

	if (ImGui::Button("Save Scene"))
	{
		OPENFILENAME ofn;       // ���� ���̾�α� ����ü
		TCHAR szFile[260];      // ������ ������ ��θ� ������ ����
		HWND hwnd = NULL;       // �θ� â�� �ڵ� (NULL�� �����ϸ� �� â�� �θ� ��)
		std::wstring path = gPathManager->GetResourcePath();
		path += L"\\Scene";
		// ���� ���̾�α� ����ü �ʱ�ȭ
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = TEXT("Text Files (*.scene)\0*.TXT\0All Files (*.*)\0*.*\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = path.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

		// ���� ���̾�α� ����
		if (GetSaveFileName(&ofn) == TRUE) {
			// ����ڰ� ������ �����ϰ� Ȯ���� ������ ��
			MessageBox(NULL, ofn.lpstrFile, TEXT("Selected File"), MB_OK);

			//FILE* file = nullptr;
			//errno_t err = _wfopen_s(&file, ofn.lpstrFile, L"wb");
			//(void)err;
			gCurrentScene->Save(ofn.lpstrFile);
			//gameObject->Save(file);
			//fclose(file);
		}
		else {
			// ����ڰ� ��Ҹ� ������ ��
			MessageBox(NULL, TEXT("No file selected!"), TEXT("Error"), MB_OK | MB_ICONERROR);
		}
	}

	if (ImGui::Button("Load Scene"))
	{

		OPENFILENAME ofn;       // ���� ���̾�α� ����ü
		TCHAR szFile[260];      // ������ ������ ��θ� ������ ����
		HWND hwnd = NULL;       // �θ� â�� �ڵ� (NULL�� �����ϸ� �� â�� �θ� ��)
		std::wstring path = gPathManager->GetResourcePath();
		path += L"\\Scene";
		// ���� ���̾�α� ����ü �ʱ�ȭ
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = TEXT("All Files (*.*)\0*.*\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = path.c_str();
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// ���� ���̾�α� ����
		if (GetOpenFileName(&ofn) == TRUE) {
			// ����ڰ� ������ �����ϰ� Ȯ���� ������ ��
			MessageBox(NULL, ofn.lpstrFile, TEXT("Selected File"), MB_OK);
	
			//������ ���� �ε��Ѵ�
			Scene* scene = new Scene();
			scene->TurnOffScript();
			scene->Load(ofn.lpstrFile);
			SceneManager::GetInstance()->RegisterLoadScene(scene);
			//reset
			bStartGame = true;
			//static_cast<InspectorUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("InspectorUI"))->UnRegister();
			//gameObject->Save(file);
			//fclose(file);
		}
		else {
			// ����ڰ� ��Ҹ� ������ ��
			MessageBox(NULL, TEXT("No file selected!"), TEXT("Error"), MB_OK | MB_ICONERROR);
		}
	}

	

	if (bStartGame)
	{
		if (ImGui::Button("Start Game"))
		{
			bStartGame = false;

			//������� �����ϰ� �ε���
			std::wstring relativePath = gPathManager->GetResourcePath();
			relativePath += L"\\Scene\\Editor\\main.scene";
			gCurrentScene->Save(relativePath);

			//�÷���
			Scene* scene = new Scene();
			scene->TurnOnScript();

			scene->Load(relativePath);
			
			SceneManager::GetInstance()->RegisterLoadScene(scene);
			//static_cast<InspectorUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("InspectorUI"))->UnRegister();
		}
	}
	else
	{
		if (ImGui::Button("End Game"))
		{
			bStartGame = true;
			//�������� �ε���

			std::wstring relativePath = gPathManager->GetResourcePath();
			relativePath += L"\\Scene\\Editor\\main.scene";

			//�����;� �ε�
			Scene* scene = new Scene();
			scene->TurnOffScript();

			scene->Load(relativePath);
			SceneManager::GetInstance()->RegisterLoadScene(scene);
			//static_cast<InspectorUI*>(PanelUIManager::GetInstance()->FindPanelUIOrNull("InspectorUI"))->UnRegister();
		}
	}

	ImGui::End();
}
