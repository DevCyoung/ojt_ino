#include "pch.h"
#include "GameObject.h"
#include "Transform.h"
GameObject* CreateGameObject()
{
	GameObject* obj = new GameObject();
	obj->AddComponent<Transform>();
	return obj;
}

GameObject* CreateGameObjectNoTransform()
{
	GameObject* obj = new GameObject();	
	return obj;
}
