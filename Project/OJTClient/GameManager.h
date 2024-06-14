#pragma once

class SceneRenderHelper;
class GameObject;

class GameManager
{
public:
	PUBLIC_SINGLETON_DECLARE(GameManager)
public:
	SceneRenderHelper* mSceneRenderHelperA;
	SceneRenderHelper* mSceneRenderHelperB;
	GameObject* mCarA;
	GameObject* mCarB;
};

