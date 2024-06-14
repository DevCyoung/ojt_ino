#pragma once

class Collider2D;
class Scene;

class RenderTargetRenderer;
class Physics2D;
class GameObject;

enum class eLayerType;

class GameSystem
{
public:
	GameSystem(Scene* const scene);
	virtual ~GameSystem();
	GameSystem(const GameSystem&) = delete;
	GameSystem& operator=(const GameSystem&) = delete;

public:
	Physics2D*  GetPhysics2D() { return mPhysics2D; }
	RenderTargetRenderer* GetRenderTargetRenderer() { return mRenderTargetRenderer; }

	GameObject* FindGameObjectOrNull(const std::wstring& name, const eLayerType type) const;
	GameObject* FindGameObjectOrNull(const std::wstring& name) const;

	GameObject* FindGameObject(const std::wstring& name, const eLayerType type) const;
	GameObject* FindGameObject(const std::wstring& name) const;

	std::vector<GameObject*> FindGameObjects(const std::wstring& name, const eLayerType type) const;
	std::vector<GameObject*> FindGameObjects(const std::wstring& name) const;

	Scene* GetScene() const { return mScene; }

private:
	Scene* mScene;
	Physics2D* mPhysics2D;
	RenderTargetRenderer* mRenderTargetRenderer;
};