#pragma once
#include "Resource.h"
#include "Layer.h"
#include "EnumLayer.h"

#include <source_location>

class GameObject;
class CollisionManagement2D;
class GameSystem;


struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

class Scene : public Resource
{
	friend class SceneManager;
	friend class CollisionManagement2D;

public:
	Scene();
	//Scene(const Scene& otehr);
	virtual ~Scene();
	//virtual Scene* Clone() = 0;
	virtual Scene* Clone();

	void RegisterEventAddGameObject(GameObject* const gameObject, const eLayerType layerType, const bool bInit = true,
		const std::source_location& location = std::source_location::current());
	void RegisterEventSetDestroy(GameObject* const gameObject,
		const std::source_location& location = std::source_location::current());

	const Layer& GetLayer(const eLayerType layerType) const { return mLayers[static_cast<UINT>(layerType)]; }
	Layer& GetLayer(const eLayerType layerType) { return mLayers[static_cast<UINT>(layerType)]; }

	void AddGameObject(GameObject* const gameObject, const eLayerType layerType);

	//RenderTargetRenderer* GetRenderTargetRenderer() const { return mRenderTargetRenderer; }
	GameSystem* GetGameSystem() const { return mGameSystem; }
	CollisionManagement2D* GetCollisionManagement2D() const { return mCollisionManagement2D; }

	virtual HRESULT Save(const std::wstring& filePath);
	virtual HRESULT Load(const std::wstring& filePath) override;

	const Vector4& GetBackgroundColor() const { return mBackgroundColor; }
	void SetBackgroundColor(const Vector4& color) { mBackgroundColor = color; }

	virtual void Render(const UINT renderTargetWidth,
		const UINT renderTargetHeight,
		ID3D11RenderTargetView** const ppRenderTargetView,
		ID3D11DepthStencilView* const depthStencilView) const;

	GameObject* FindGameObjectOrNull(const std::wstring& name);

	void TurnOnScript() { mbExcuteScript = true; }
	void TurnOffScript() { mbExcuteScript = false; }
	bool IsExcuteScript() const { return mbExcuteScript; }
private:
	enum class eEvent
	{
		AddGameObject,
		DestroyGameObject,
		LoadScene,
		End,
	};

	struct tEventMessageScene
	{
		eEvent EventOfSceneType;
		GameObject* EventGameObject;
		eLayerType LayerType;
		bool	bValue;
		std::source_location ErrorHint;
	};

protected:
	virtual void initialize();
	virtual void update();
	virtual void fixedUpdate();
	virtual void lateUpdate();

	//virtual void physicsUpdate
	

	virtual void renderFlush();
	virtual void eventUpdate();

protected:
	CollisionManagement2D* mCollisionManagement2D;	

private:
	GameSystem* mGameSystem;
	Layer mLayers[static_cast<UINT>(eLayerType::End)];
	std::vector<tEventMessageScene> mEventMessages;
	std::vector<GameObject*> mGarbages;
	Vector4 mBackgroundColor;

	bool mbExcuteScript;
};
