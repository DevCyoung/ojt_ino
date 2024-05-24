#pragma once
#include "Resource.h"

class GameObject;

class Prefab : public Resource
{
public:
	Prefab(GameObject* gameObject);
	virtual ~Prefab();
	Prefab(const Prefab&) = delete;
	Prefab& operator=(const Prefab&) = delete;

	virtual HRESULT Load(const std::wstring& filePath) override final;

	GameObject* GetGameObject() const { Assert(mRoot, ASSERT_MSG_NULL); return mRoot; }

private:
	GameObject* mRoot;
};

