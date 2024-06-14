#include "pch.h"
#include "Layer.h"
#include "GameObject.h"

Layer::Layer()
	:mGameObjects()
{
}

Layer::~Layer()
{
	mem::del::DeleteVectorElements(&mGameObjects);
}

void Layer::Save(FILE* const file)
{
	UINT objectCount = static_cast<UINT>(mGameObjects.size());
	fwrite(&objectCount, sizeof(UINT), 1, file);

	for (GameObject* const gameObject : mGameObjects)
	{		
		gameObject->Save(file);
	}
}

void Layer::Load(FILE* const file)
{
	UINT objectCount = 0;
	fread(&objectCount, sizeof(UINT), 1, file);

	for (UINT i = 0; i < objectCount; i++)
	{
		GameObject* obj = new GameObject();
		obj->Load(file);
		AddGameObject(obj);
	}
}

void Layer::initialize()
{
	for (GameObject* const gameObject : mGameObjects)
	{
		if (gameObject->mState == GameObject::eState::Active)
		{
			gameObject->initialize();
		}
	}
}

void Layer::update()
{
	for (GameObject* const gameObject : mGameObjects)
	{
		if (gameObject->mState == GameObject::eState::Active)
		{
			gameObject->update();
		}
	}
}

void Layer::fixedUpdate()
{
	for (GameObject* const gameObject : mGameObjects)
	{
		if (gameObject->mState == GameObject::eState::Active)
		{
			gameObject->fixedUpdate();
		}
	}
}

void Layer::lateUpdate()
{
	for (GameObject* const gameObject : mGameObjects)
	{
		if (gameObject->mState == GameObject::eState::Active)
		{
			gameObject->lateUpdate();
		}
	}	
}

void Layer::lastUpdate()
{
	for (GameObject* const gameObject : mGameObjects)
	{
		if (gameObject->mState == GameObject::eState::Active)
		{
			gameObject->lastUpdate();
		}
	}
}

void Layer::DetatchGameObject(GameObject* gameobject)
{
	std::vector<GameObject*>::iterator iter = mGameObjects.begin();
	for (; iter != mGameObjects.end(); ++iter)
	{
		if (*iter == gameobject)
		{
			iter = mGameObjects.erase(iter);
			break;
		}
	}

	//Assert(iter != mGameObjects.end(), ASSERT_MSG_INVALID);
}