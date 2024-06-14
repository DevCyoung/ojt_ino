#pragma once
#include "Resource.h"

class Mesh;
class Material;
class GameObject;
class MeshData : public Resource
{
public:
	MeshData();
	virtual ~MeshData();
	MeshData(const MeshData&) = delete;
	MeshData& operator=(const MeshData&) = delete;

	// Resource을(를) 통해 상속됨
	HRESULT Load(const std::wstring& filePath) override;
	HRESULT Save(const std::wstring& name) override;

	GameObject* Instantiate();

	void SetMesh(Mesh* mesh) { mMesh = mesh; }
	void SetMaterial(std::vector<Material*> materials) { mMaterials = materials;  }
	void SetMeshDataName(std::wstring meshDataName) { mMeshDataName = meshDataName; }
	void AddChildMeshData(MeshData* meshData) { mChildMeshDatas.push_back(meshData); }

	const std::wstring& GetMeshDataName() { return mMeshDataName; }

private:
	Mesh*						mMesh;
	std::wstring				mMeshDataName;
	std::vector<MeshData*>		mChildMeshDatas;
	std::vector<Material*>		mMaterials;
};

