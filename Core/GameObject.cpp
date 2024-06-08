#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
#include "ScriptComponent.h"
#include "EnumLayer.h"
#include "SceneManager.h"
//#include "BuildSetting.h"
//#include <Helper\FileHelper.h>

const wchar_t* GetScriptComponentName(const eScriptComponentType type);
ScriptComponent* CreateScriptComponentByName(const std::wstring& scriptcomponentName);

GameObject::GameObject()
	: mEngineComponents{0,}
	, mScriptComponents()
	, mLayerType(eLayerType::Default)
	, mState(eState::Active)
	, mParent(nullptr)
	, mChildObjects()
	, mGameSystem(nullptr)
{	
}

GameObject::~GameObject()
{
	mem::del::DeleteVectorElements(&mScriptComponents);
	mem::del::DeletePointerArrayElements(&mEngineComponents);
	mem::del::DeleteVectorElements(&mChildObjects);
}

GameObject::GameObject(const GameObject& other)
	: mEngineComponents{0, }
	, mScriptComponents()
	, mLayerType(other.mLayerType)
	, mState(eState::Active)
	, mParent(nullptr)
	, mChildObjects()
	, mGameSystem(nullptr)
{
	for (UINT i = 0; i < static_cast<UINT>(eComponentType::End); i++)
	{
		if (other.mEngineComponents[i])
		{
			AddComponent(other.mEngineComponents[i]->Clone());			
		}
	}

	for (UINT i = 0; i < mScriptComponents.size(); i++)
	{
		AddComponent(other.mScriptComponents[i]->Clone());
	}

	for (const GameObject* obj : other.mChildObjects)
	{
		GameObject* copyObject = new GameObject(*obj);
		mChildObjects.push_back(copyObject);
	}
}

void GameObject::initialize()
{
	Assert(mGameSystem, ASSERT_MSG_NULL);

	for (Component* const component : mEngineComponents)
	{
		if (component)
		{
			component->initialize();
		}
	}

#ifdef EDITOR_MODE
	if (gCurrentScene->IsExcuteScript())
	{
		for (ScriptComponent* const script : mScriptComponents)
		{
			script->initialize();
		}
	}
#else
	for (ScriptComponent* const script : mScriptComponents)
	{
		script->initialize();
	}
#endif // !1	

	for (GameObject* childObj : mChildObjects)
	{
		childObj->mGameSystem = mGameSystem;
		childObj->initialize();
	}
}

void GameObject::update()
{
	for (Component* const component : mEngineComponents)
	{
		if (component)
		{
			component->update();
		}
	}


#ifdef EDITOR_MODE
	if (gCurrentScene->IsExcuteScript())
	{
		for (ScriptComponent* const script : mScriptComponents)
		{
			script->update();
		}
	}
#else
	for (ScriptComponent* const script : mScriptComponents)
	{
		script->update();
	}
#endif // !1	

	for (GameObject* childobj : mChildObjects)
	{
		childobj->update();
	}
}

void GameObject::fixedUpdate()
{
	for (Component* const component : mEngineComponents)
	{
		if (component)
		{
			component->fixedUpdate();
		}
	}

#ifdef EDITOR_MODE
	if (gCurrentScene->IsExcuteScript())
	{
		for (ScriptComponent* const script : mScriptComponents)
		{
			script->fixedUpdate();
		}
	}
#else
	for (ScriptComponent* const script : mScriptComponents)
	{
		script->fixedUpdate();
	}
#endif // !1	



	for (GameObject* childobj : mChildObjects)
	{
		childobj->fixedUpdate();
	}
}


void GameObject::lateUpdate()
{
	for (Component* const component : mEngineComponents)
	{
		if (component)
		{
			component->lateUpdate();
		}
	}

#ifdef EDITOR_MODE
	if (gCurrentScene->IsExcuteScript())
	{
		for (ScriptComponent* const script : mScriptComponents)
		{
			script->lateUpdate();
		}
	}
#else
	for (ScriptComponent* const script : mScriptComponents)
	{
		script->lateUpdate();
	}
#endif // !1	

	for (GameObject* childobj : mChildObjects)
	{
		childobj->lateUpdate();
	}
}


void GameObject::lastUpdate()
{
	for (Component* const component : mEngineComponents)
	{
		if (component)
		{
			component->lastUpdate();
		}
	}


#ifdef EDITOR_MODE
	if (gCurrentScene->IsExcuteScript())
	{
		for (ScriptComponent* const script : mScriptComponents)
		{
			script->lastUpdate();
		}
	}
#else
	for (ScriptComponent* const script : mScriptComponents)
	{
		script->lastUpdate();
	}
#endif // !1	


	for (GameObject* childobj : mChildObjects)
	{
		childobj->lastUpdate();
	}
}

GameObject* GameObject::FindChidOrNull(const std::wstring& name)
{
	GameObject* obj = nullptr;

	for (GameObject* child : mChildObjects)
	{
		if (child->GetName() == name)
		{
			obj = child;			
		}
		else
		{
			obj = obj->FindChidOrNull(name);
		}

		if (obj)
		{
			break;
		}
	}

	return obj;
}

GameObject* GameObject::FindChidOrNull(GameObject* childObj)
{
	GameObject* obj = nullptr;

	for (GameObject* child : mChildObjects)
	{
		if (child == childObj)
		{
			obj = child;
		}
		else
		{
			obj = child->FindChidOrNull(childObj);
		}

		if (obj)
		{
			break;
		}
	}

	return obj;
}

void GameObject::DetatchChild(GameObject* childObj)
{	
	if (!FindChidOrNull(childObj))
	{
		return;
	}

	Assert(childObj->mParent == this, ASSERT_MSG_INVALID);

	//부모는 child를 떼어낸다.
	std::vector<GameObject*>::iterator iter = childObj->mParent->mChildObjects.begin();
	for (; iter != childObj->mParent->mChildObjects.end(); ++iter)
	{
		if (childObj == *iter)
		{
			childObj->mParent->mChildObjects.erase(iter);
			break;
		}
	}

	childObj->mParent = nullptr;
}

void GameObject::SetChild(GameObject* const childObj)
{
	Assert(childObj, ASSERT_MSG_NULL);

	//부모를 넣으려고 한다면
	if (childObj->FindChidOrNull(this))
	{
		return;
	}
	else if (childObj->mParent) //부모는 child를 떼어낸다.
	{
		GameObject* parentObject = childObj->mParent;
		std::vector<GameObject*>::iterator iter = parentObject->mChildObjects.begin();		
		for (; iter != parentObject->mChildObjects.end(); ++iter)
		{
			if (childObj == *iter)
			{
				iter = parentObject->mChildObjects.erase(iter);				
				parentObject = nullptr;
				break;
			}
		}
		
		Assert(!parentObject, ASSERT_MSG_NULL);
	}
	else if (SceneManager::GetInstance()->GetCurrentSceneOrNull())
	{
		eLayerType layerType = childObj->GetLayer();
		Layer& layer = gCurrentScene->GetLayer(layerType);
		layer.DetatchGameObject(childObj);
	}

	childObj->mParent = this;
	mChildObjects.push_back(childObj);	
}

void GameObject::SetParent(GameObject* const parent)
{
	if (parent)
	{
		parent->SetChild(this);
	}
	else if (mParent)
	{
		mParent->DetatchChild(this);
		gCurrentScene->RegisterEventAddGameObject(this, mLayerType, false);
	}
}

void GameObject::AddComponent(ScriptComponent* const scriptComponent)
{
	Assert(scriptComponent, ASSERT_MSG_NULL);
	Assert(!(scriptComponent->mOwner), ASSERT_MSG_NOT_NULL);
	Assert(!(GetComponentOrNull(scriptComponent->GetScriptType())), ASSERT_MSG_NOT_NULL);

	scriptComponent->mOwner = this;

	mScriptComponents.push_back(scriptComponent);
}

void GameObject::AddComponent(Component* const component)
{
	Assert(component, ASSERT_MSG_NULL);
	Assert(!(component->mOwner), ASSERT_MSG_NOT_NULL);

	component->mOwner = this;

	if (component->GetType() == eComponentType::ScriptComponent)
	{
		ScriptComponent* const scriptComponent = dynamic_cast<ScriptComponent*>(component);

		Assert(scriptComponent, ASSERT_MSG_NULL);

		AddComponent(scriptComponent);
	}
	else
	{
		Assert(!mEngineComponents[static_cast<UINT>(component->GetType())], ASSERT_MSG_NOT_NULL);

		mEngineComponents[static_cast<UINT>(component->GetType())] = component;
	}
}

Component* GameObject::GetComponentOrNull(const eComponentType componentType) const
{
	Assert(static_cast<UINT>(eComponentType::End) > static_cast<UINT>(componentType),
		ASSERT_MSG_INVALID);

	return mEngineComponents[static_cast<UINT>(componentType)];
}

ScriptComponent* GameObject::GetComponentOrNull(const eScriptComponentType scriptComponentType) const
{
	ScriptComponent* component = nullptr;

	for (ScriptComponent* const curScript : mScriptComponents)
	{
		if (curScript->GetScriptType() == scriptComponentType)
		{
			component = curScript;

			break;
		}
	}

	return component;
}

Component* GameObject::GetComponent(const eComponentType componentType) const
{
	Component* const component = GetComponentOrNull(componentType);

	Assert(component, ASSERT_MSG_NULL);

	return component;
}

ScriptComponent* GameObject::GetComponent(const eScriptComponentType scriptComponentType) const
{
	ScriptComponent* const scriptComponent = GetComponentOrNull(scriptComponentType);

	Assert(scriptComponent, ASSERT_MSG_NULL);

	return scriptComponent;
}

void GameObject::Save(FILE* file)
{
	SaveWString(GetName(), file);

	UINT layerType = static_cast<UINT>(mLayerType);
	fwrite(&layerType, sizeof(UINT), 1, file);
	
	UINT componentCount = 0;
	for (Component* const component : mEngineComponents)
	{
		if (component)
		{			
			++componentCount;			
		}
	}

	fwrite(&componentCount, sizeof(UINT), 1, file);
	for (Component* const component : mEngineComponents)
	{
		if (component)
		{			
			std::wstring componentName = GetComponentName(component->GetType());
			SaveWString(componentName, file);
			component->Save(file);
		}
	}

	UINT scriptComponentCount = static_cast<UINT>(mScriptComponents.size());
	fwrite(&scriptComponentCount, sizeof(UINT), 1, file);

	for (ScriptComponent* const script : mScriptComponents)
	{
		std::wstring componentName = GetScriptComponentName(script->GetScriptType());
		SaveWString(componentName, file);
		script->Save(file);
	}

	UINT childObjectCount = static_cast<UINT>(mChildObjects.size());
	fwrite(&childObjectCount, sizeof(UINT), 1, file);

	for (GameObject* childobj : mChildObjects)
	{
		childobj->Save(file);
	}
}

void GameObject::Load(FILE* const file)
{
	std::wstring name;
	LoadWString(&name, file);
	SetName(name);

	UINT layerType = static_cast<UINT>(mLayerType);
	fread(&layerType, sizeof(UINT), 1, file);
	mLayerType = static_cast<eLayerType>(layerType);

	UINT componentCount = 0;
	fread(&componentCount, sizeof(UINT), 1, file);

	for (UINT i = 0; i < componentCount; ++i)
	{
		std::wstring componentName = L"";
		LoadWString(&componentName, file);
		
		Component* component = CreateComponentByName(componentName);		
		component->Load(file);

		//if (component->gett)
		AddComponent(component);
	}

	UINT scriptComponentCount = 0;
	fread(&scriptComponentCount, sizeof(UINT), 1, file);

	for (UINT i = 0; i < scriptComponentCount; i++)
	{
		std::wstring scriptComponentName = L"";
		LoadWString(&scriptComponentName, file);
		ScriptComponent* scriptComponent = CreateScriptComponentByName(scriptComponentName);
		scriptComponent->Load(file);
		AddComponent(scriptComponent);
	}

	UINT childObjectCount = 0;
	fread(&childObjectCount, sizeof(UINT), 1, file);

	for (UINT i = 0; i < childObjectCount; i++)
	{
		GameObject* childObj = new GameObject();
		childObj->Load(file);
		SetChild(childObj);
	}
}
