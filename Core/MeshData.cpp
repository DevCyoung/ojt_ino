#include "pch.h"
#include "MeshData.h"
#include "Mesh.h"
#include "Material.h"
#include "PathManager.h"

#include "FBXLoader.h"
#include "PathManager.h"
#include "FBXLoadManager.h"
#include "StructVertex.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include "Mesh.h"
#include "Shader.h"
#include "ResourceManager.h"
#include "EnumRenderType.h"
#include "Material.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "ResourceManager.h"
#include "Texture.h"
#include <Shlwapi.h>
#include "StructuredBuffer.h"
#include "Animator3D.h"
#include "GameObject.h"
#include "PathManager.h"
//#include  <Helper\FileHelper.h>
#include "Animation3DController.h"
#include "Builder.h"

MeshData::MeshData()
	: Resource(eResourceType::MeshData)
	, mMesh(nullptr)
	, mMeshDataName(L"None")
	, mChildMeshDatas()
	, mMaterials()

{
}
MeshData::~MeshData()
{
}

#include <iostream>
#include <windows.h>

static bool CreateFoldersRecursive(const wchar_t* folderPath) {
	// 디렉토리가 이미 존재하면 성공으로 간주
	if (CreateDirectory(folderPath, NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
		return true;
	}

	// 디렉토리가 존재하지 않는 경우, 상위 디렉토리를 먼저 만듭니다.
	std::wstring pathStr(folderPath);
	size_t pos = pathStr.find_last_of(L"/\\");
	if (pos != std::wstring::npos) {
		// 상위 디렉토리를 먼저 만들기 위해 재귀 호출
		bool success = CreateFoldersRecursive(pathStr.substr(0, pos).c_str());
		if (!success) {
			return false; // 상위 디렉토리 생성 실패
		}
	}

	// 마지막 폴더를 생성
	return (CreateDirectory(folderPath, NULL) || GetLastError() == ERROR_ALREADY_EXISTS);
}

HRESULT MeshData::Load(const std::wstring& filePath)
{
	FILE* file = nullptr;
	errno_t err = _wfopen_s(&file, filePath.c_str(), L"rb");
	(void)err;

	//Assert(err, ASSERT_MSG_INVALID);
	Assert(file, ASSERT_MSG_NULL);

	LoadWString(&mMeshDataName, file);

	std::wstring meshPath;
	LoadWString(&meshPath, file);

	UINT bMesh;
	fread(&bMesh, sizeof(UINT), 1, file);
	if (bMesh)
	{
		mMesh = gResourceManager->FindAndLoadOrNull<Mesh>(meshPath);
	}

	UINT size;
	fread(&size, sizeof(UINT), 1, file);


	for (UINT i = 0; i < size; ++i)
	{
		std::wstring matPath;
		LoadWString(&matPath, file);
		Material* mat = gResourceManager->FindAndLoadOrNull<Material>(matPath);
		mMaterials.push_back(mat);
	}

	UINT childSize;
	fread(&childSize, sizeof(UINT), 1, file);

	for (UINT i = 0; i < childSize; ++i)
	{
		std::wstring matPath;
		LoadWString(&matPath, file);
		MeshData* mat = gResourceManager->FindAndLoadOrNull<MeshData>(matPath);
		mChildMeshDatas.push_back(mat);
	}

	fclose(file);
	return E_NOTIMPL;
}

HRESULT MeshData::Save(const std::wstring& relativePath)
{
	std::wstring resPath = gPathManager->GetResourcePath();
	const std::wstring& meshDataFilePath = resPath + L"\\MeshData" + relativePath + L".mesh_data";

	FILE* file = nullptr;
	//CreateFoldersRecursive(meshDataFilePath.c_str);
	errno_t err = _wfopen_s(&file, meshDataFilePath.c_str(), L"wb");
	(void)err;

	Assert(file, ASSERT_MSG_NULL);

	SaveWString(mMeshDataName, file);

	std::wstring meshPath = L"\\Mesh";
	meshPath += relativePath;
	meshPath += L".mesh";
	SaveWString(meshPath, file);

	UINT bMesh = !(mMesh == nullptr);
	fwrite(&bMesh, sizeof(UINT), 1, file);
	if (mMesh)
	{
		mMesh->Save(meshPath);
	}

	UINT size = static_cast<UINT>(mMaterials.size());
	fwrite(&size, sizeof(UINT), 1, file);
	for (int i = 0; i < mMaterials.size(); ++i)
	{
		std::wstring matPath = L"\\Material";
		matPath += relativePath;
		matPath += L"_";
		matPath += std::to_wstring(i);
		matPath += L".mat";

		SaveWString(matPath, file);
		mMaterials[i]->Save(matPath);
	}

	//자식들저장
	UINT msize = static_cast<UINT>(mChildMeshDatas.size());
	fwrite(&msize, sizeof(UINT), 1, file);

	for (UINT i = 0; i < msize; ++i)
	{
		std::wstring cnildMeshDataPath = L"\\MeshData" + relativePath + L"_child_" + std::to_wstring(i) + L".mesh_data";
		SaveWString(cnildMeshDataPath, file);

		cnildMeshDataPath = relativePath + L"_child_" + std::to_wstring(i);
		mChildMeshDatas[i]->Save(cnildMeshDataPath);

	}

	fclose(file);
	return E_NOTIMPL;
}

GameObject* MeshData::Instantiate()
{
	GameObject* root = CreateGameObject();
	{		
	}
	{
		GameObject* meshRoot = CreateGameObject();
		meshRoot->SetName(L"MeshRoot");

		for (MeshData* childMeshData : mChildMeshDatas)
		{
			GameObject* childObj = CreateGameObject();
			childObj->SetName(childMeshData->GetMeshDataName());
			childObj->AddComponent<MeshRenderer>();

			childObj->GetComponent<MeshRenderer>()->SetMesh(childMeshData->mMesh);
			for (int i = 0; i < childMeshData->mMaterials.size(); ++i)
			{
				childObj->GetComponent<MeshRenderer>()->SetMaterial(childMeshData->mMaterials[i], i);
			}

			if (childMeshData->mMesh->IsAnimMesh())
			{
				if (!root->GetComponentOrNull<Animation3DController>())
				{					
					root->AddComponent<Animation3DController>();
					root->GetComponent< Animation3DController>()->SetAnimClip(*childMeshData->mMesh->GetAnimClip());
				}

				childObj->AddComponent<Animator3D>();
				childObj->GetComponent<Animator3D>()->SetBones(*childMeshData->mMesh->GetBones());
				//childObj->GetComponent<Animator3D>()->SetAnimClip(childMeshData->mMesh->GetAnimClip());

				root->GetComponent<Animation3DController>()->AddAnimator3D(childObj->GetComponent<Animator3D>());				
			}

			meshRoot->SetChild(childObj);
		}

		root->SetChild(meshRoot);
	}


	return root;
}
