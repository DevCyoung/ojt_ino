#include "pch.h"
#include "GameSystem.h"
#include "Physics2D.h"
#include "RenderTargetRenderer.h"
#include "Layer.h"
#include "GameObject.h"
#include "Scene.h"

GameSystem::GameSystem(Scene* const scene)
	: mScene(scene)
	, mPhysics2D(new Physics2D(scene))
	, mRenderTargetRenderer(new RenderTargetRenderer())
{
}

GameSystem::~GameSystem()
{
	DELETE_POINTER_NOT_NULL(mRenderTargetRenderer);
	DELETE_POINTER_NOT_NULL(mPhysics2D);
}

GameObject* GameSystem::FindGameObjectOrNull(const std::wstring& name, const eLayerType type) const
{
	const Layer& layer = mScene->GetLayer(type);

	GameObject* result = nullptr;

	for (GameObject* const gameObject : layer.GetGameObjects())
	{
		if (gameObject->GetName() == name)
		{
			result = gameObject;
			break;
		}
	}

	return result;
}

GameObject* GameSystem::FindGameObjectOrNull(const std::wstring& name) const
{
	GameObject* result = nullptr;

	for (int i = 0; i < static_cast<UINT>(eLayerType::End); ++i)
	{
		eLayerType type = static_cast<eLayerType>(i);
		result =  FindGameObjectOrNull(name, type);

		if (result)
		{
			break;
		}
	}

	return result;
}

GameObject* GameSystem::FindGameObject(const std::wstring& name, const eLayerType type) const
{	
	GameObject* result = FindGameObjectOrNull(name ,type);

	Assert(result, ASSERT_MSG_NULL);
	return result;
}

GameObject* GameSystem::FindGameObject(const std::wstring& name) const
{
	GameObject* result = FindGameObjectOrNull(name);

	Assert(result, ASSERT_MSG_NULL);
	return result;
}

std::vector<GameObject*> GameSystem::FindGameObjects(const std::wstring& name, const eLayerType type) const
{
	std::vector<GameObject*> result;

	result.reserve(100);

	const Layer& layer = mScene->GetLayer(type);		

	for (GameObject* const gameObject : layer.GetGameObjects())
	{
		if (gameObject->GetName() == name)
		{
			result.push_back(gameObject);			
		}
	}

	return result;
}

std::vector<GameObject*> GameSystem::FindGameObjects(const std::wstring& name) const
{
	std::vector<GameObject*> result;

	result.reserve(100);

	for (int i = 0; i < static_cast<UINT>(eLayerType::End); ++i)
	{
		eLayerType type = static_cast<eLayerType>(i);
		std::vector<GameObject*> temp = FindGameObjects(name, type);
		result.insert(result.end(), temp.begin(), temp.end());
	}

	return result;
}
