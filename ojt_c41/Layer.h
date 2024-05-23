#pragma once
class GameObject;

class Layer
{
	friend class Scene;

public:
	const std::vector<GameObject*>& GetGameObjects() const { return mGameObjects; }
	std::vector<GameObject*>& GetGameObjects() { return mGameObjects; }

	void DetatchGameObject(GameObject* gameobject);

private:
	Layer();
	virtual ~Layer();
	Layer(const Layer&) = delete;
	Layer& operator=(const Layer&) = delete;

	void Save(FILE* const file);
	void Load(FILE* const file);
	void AddGameObject(GameObject* obj) { mGameObjects.push_back(obj); }

private:
	void initialize();
	void update();
	void fixedUpdate();
	void lateUpdate();
	void lastUpdate();

private:
	std::vector<GameObject*> mGameObjects;
};
