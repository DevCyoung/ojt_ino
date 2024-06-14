#pragma once
class SceneRenderHelper;
class GameObject;

class GameManager
{
public:
	static void initialize()
	{
		Assert(!sInstance, ASSERT_MSG_NOT_NULL);
		sInstance = new GameManager();
	}
	static void deleteInstance()
	{
		Assert(sInstance, ASSERT_MSG_NULL);
		delete sInstance;
		sInstance = nullptr;
	}
public:
	static GameManager* GetInstance()
	{
		Assert(sInstance, ASSERT_MSG_NULL);
		return sInstance;
	}
private:
	GameManager();
	virtual ~GameManager();
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
private:
	inline static GameManager* sInstance = nullptr;

public:
	SceneRenderHelper* mSceneRenderHelperA;
	SceneRenderHelper* mSceneRenderHelperB;
	GameObject* mCarA;
	GameObject* mCarB;
};

