#include "pch.h"
#include "RenderComponent.h"
#include "RenderTargetRenderer.h"
#include "GameObject.h"
#include "GameSystem.h"
#include "Mesh.h"
//#include "Helper\FileHelper.h"
#include "Material.h"
#include "ResourceManager.h"
RenderComponent::RenderComponent()
	: Component(eComponentType::RenderComponent)
	, mbVisible(true)
	, mMesh(nullptr)
	, mMaterials()
{
	//mMaterials.resize(sizeof(UINT));
}

RenderComponent::RenderComponent(const eComponentType componentType)
	: Component(componentType)
	, mbVisible(true)
	, mMesh(nullptr)
	, mMaterials()
{
}

void RenderComponent::SetMesh(Mesh* const mesh)
{
	Assert(mesh, ASSERT_MSG_NULL);
	mMesh = mesh;
	mMaterials.resize(mMesh->GetIndexBufferCount());

}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::initialize()
{
}

void RenderComponent::Save(FILE* const file)
{
	Component::Save(file);
	fwrite(&mbVisible, sizeof(bool), 1, file);

	bool bMesh = nullptr != mMesh;
	fwrite(&bMesh, sizeof(bool), 1, file);
	if (bMesh)
	{
		std::wstring pathOrName = mMesh->GetRelativePathorName();
		SaveWString(pathOrName, file);
	}

	UINT materialCount = static_cast<UINT>(mMaterials.size());
	fwrite(&materialCount, sizeof(UINT), 1, file);
	for (UINT i = 0; i < materialCount; ++i)
	{
		std::wstring pathOrName = mMaterials[i]->GetRelativePathorName();
		SaveWString(pathOrName, file);
	}
}

void RenderComponent::Load(FILE* const file)
{
	Component::Load(file);
	fread(&mbVisible, sizeof(bool), 1, file);

	bool bMesh = nullptr != mMesh;
	fread(&bMesh, sizeof(bool), 1, file);
	if (bMesh)
	{
		std::wstring pathOrName;
		LoadWString(&pathOrName, file);
		mMesh = gResourceManager->FindAndLoad<Mesh>(pathOrName);
	}

	UINT materialCount = static_cast<UINT>(mMaterials.size());
	fread(&materialCount, sizeof(UINT), 1, file);
	for (UINT i = 0; i < materialCount; ++i)
	{
		std::wstring pathOrName;
		LoadWString(&pathOrName, file);		
		Material* mat = gResourceManager->FindAndLoad<Material>(pathOrName);
		mMaterials.push_back(mat);
	}
}

void RenderComponent::update()
{
	if (GetOwner()->GetState() == GameObject::eState::Active && mbVisible)
	{
		GetOwner()->GetGameSystem()->GetRenderTargetRenderer()->registerRenderComponent(this);
	}
}

void RenderComponent::lateUpdate()
{
}
