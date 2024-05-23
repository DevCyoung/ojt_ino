#pragma once
#include "Entity.h"
#include "ComponentTrait.h"
#include "EnumComponent.h"
#include "EnumLayer.h"
#include "ScriptType.h"

class ScriptComponent;
class GameSystem;

class GameObject : public Entity
{
	friend class Layer;
	friend class Scene;

public:
	enum class eState
	{
		InActive,
		Active,
		Destroy,
	};
	friend GameObject* CreateGameObject();
	friend GameObject* CreateGameObjectNoTransform();
private:	
	GameObject();
public:
	virtual ~GameObject();
	GameObject(const GameObject&);
	GameObject& operator=(const GameObject&) = delete;

public:
	template<typename T>
		requires (std::is_base_of_v<Component, T>)
	T* GetComponentOrNull() const;
	template<typename T>
		requires (is_component<T>::value)
	T* GetComponent() const;

	template<typename T>
		requires (is_component<T>::value)
	T* FindParentByComponent() const;

	Component* GetComponentOrNull(const eComponentType componentType) const;
	ScriptComponent* GetComponentOrNull(const eScriptComponentType scriptComponentType) const;

	Component* GetComponent(const eComponentType componentType) const;
	ScriptComponent* GetComponent(const eScriptComponentType scriptComponentType) const;

	const std::vector<ScriptComponent*>& GetScriptComponents() const { return mScriptComponents; }
	std::vector<ScriptComponent*>& GetScriptComponents() { return mScriptComponents; }

	eState GetState() const { return mState; }
	eLayerType GetLayer() const { return mLayerType; }

	GameObject* GetParentOrNull() const { return mParent; }
	GameSystem* GetGameSystem() const { return mGameSystem; }

	GameObject* FindChidOrNull(const std::wstring& name);
	GameObject* FindChidOrNull(GameObject* childObj);	

	void DetatchChild(GameObject* childObj);

	const std::vector<GameObject*>& GetChildObjects() const { return mChildObjects; }		

	void SetChild(GameObject* const childObj);
	void SetParent(GameObject* const parent);

	template<typename T>
		requires (is_component<T>::value)
	void AddComponent(T* const component);
	template<typename T>
		requires (is_component<T>::value)
	void AddComponent();
	void AddComponent(ScriptComponent* const component);
	void AddComponent(Component* const component);

	virtual void Save(FILE* const file);
	virtual void Load(FILE* const file);

private:
	void initialize();	
	void update();	
	void fixedUpdate();
	void lateUpdate();
	void lastUpdate();

	CLONE(GameObject);
private:
	Component* mEngineComponents[static_cast<UINT>(eComponentType::End)];
	std::vector<ScriptComponent*> mScriptComponents;
	eLayerType mLayerType;
	eState mState;
	GameObject* mParent;
	std::vector<GameObject*> mChildObjects;
	GameSystem* mGameSystem;
};

template<typename T>
	requires (is_component<T>::value)
inline void GameObject::AddComponent(T* const component)
{		
	Assert(component, ASSERT_MSG_NULL);
	Assert(!(component->mOwner), ASSERT_MSG_NOT_NULL);
	Assert(!(GetComponentOrNull<T>()), ASSERT_MSG_NOT_NULL);

	component->mOwner = this;	

	if constexpr (engine_component_trait<T>::value)
	{		
		mEngineComponents[static_cast<UINT>(engine_component_trait<T>::type)] = component;
	}
	else if constexpr (script_component_trait<T>::value)
	{		
		mScriptComponents.push_back(component);
	}
	else
	{		
		Assert(false, ASSERT_MSG_INVALID);
	}
}

template<typename T>
	requires (is_component<T>::value)
inline void GameObject::AddComponent()
{
	T* const component = new T();

	AddComponent(component);
}

template<typename T>
	requires (std::is_base_of_v<Component, T>)
inline T* GameObject::GetComponentOrNull() const
{
	T* result = nullptr;

	if constexpr (engine_component_trait<T>::value) // engine component
	{
		result = static_cast<T*>(mEngineComponents[static_cast<UINT>
			(engine_component_trait<T>::type)]);
	}
	else if constexpr (script_component_trait<T>::value) // script component
	{
		for (ScriptComponent* const scriptComponent : mScriptComponents)
		{
			if (GetScriptComponentType(scriptComponent) == script_component_trait<T>::type)
			{
				result = static_cast<T*>(scriptComponent);

				break;
			}
		}
	}
	else if constexpr (std::is_base_of_v<ScriptComponent, T>) // child script component
	{
		for (ScriptComponent* const scriptComponent : mScriptComponents)
		{
			result = dynamic_cast<T*>(scriptComponent);

			if (result)
			{
				break;
			}
		}
	}
	else
	{
		for (Component* const engineComponent : mEngineComponents) // child engine component
		{
			result = dynamic_cast<T*>(engineComponent);

			if (result)
			{
				break;
			}
		}
	}

	return result;
}

template<typename T>
	requires (is_component<T>::value)
inline T* GameObject::GetComponent() const
{	
	T* const component = GetComponentOrNull<T>();

	Assert(component, ASSERT_MSG_NULL);

	return component;
}

template<typename T>
	requires (is_component<T>::value)
inline T* GameObject::FindParentByComponent() const
{
	if (nullptr == mParent)
	{
		return nullptr;
	}

	T* component =  mParent->GetComponentOrNull<T>();
	if (component)
	{
		return component;
	}

	return mParent->FindParentByComponent<T>();
}