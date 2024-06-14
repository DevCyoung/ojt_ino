#pragma once
#include "Entity.h"
#include "EnumComponent.h"
#include "ComponentTrait.h"

class GameObject;
class ScriptComponent;
enum class eScriptComponentType;

class Component : public Entity
{
	friend class GameObject;

protected:
	Component(const eComponentType componentType);
public:
	virtual ~Component();
	Component(const Component&);
	Component& operator=(const Component&) = delete;

public:
	GameObject* GetOwner() const { Assert(mOwner, ASSERT_MSG_NULL); return mOwner; }	
	eComponentType GetType() const { return mType; }

	template<typename T, typename Object = GameObject>
		requires (std::is_base_of_v<Component, T>)
	T* GetComponentOrNull() const;
	template<typename T, typename Object = GameObject>
		requires (is_component<T>::value)
	T* GetComponent() const;

	Component* GetComponentOrNull(const eComponentType componentType) const;
	ScriptComponent* GetComponentOrNull(const eScriptComponentType scriptComponentType) const;

	Component* GetComponent(const eComponentType componentType) const;
	ScriptComponent* GetComponent(const eScriptComponentType scriptComponentType) const;

	virtual void Save(FILE* const file);
	virtual void Load(FILE* const file);

	CLONE(Component)
private:
	virtual void initialize();
	virtual void update();
	virtual void fixedUpdate();
	virtual void lateUpdate();
	virtual void lastUpdate();

private:
	eComponentType mType;
	GameObject* mOwner;
};

template<typename T, typename Object>
	requires (std::is_base_of_v<Component, T>)
inline T* Component::GetComponentOrNull() const
{	
	return static_cast<Object*>(mOwner)->GetComponentOrNull<T>();
}

template<typename T, typename Object>
	requires (is_component<T>::value)
inline T* Component::GetComponent() const
{	
	return static_cast<Object*>(mOwner)->GetComponent<T>();
}
