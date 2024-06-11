#pragma once
#include "GameObject.h"
#include "RenderTargetRenderer.h"
#include "GameSystem.h"
#include "SceneManager.h"
#include "AlphaHelper.h"
#include "Scene.h"

class Texture;

class SceneRenderHelper
{
public:
	SceneRenderHelper(const std::wstring& label, const int width, const int height);
	~SceneRenderHelper();
	SceneRenderHelper(const GameObject& other) = delete;
	SceneRenderHelper& operator=(const SceneRenderHelper& other) = delete;

	void Draw(Scene* scene);
	Texture* GetRenderTexture() const { return mRenderTex;  }
	GameObject* GetCamera() { return mCamera; }

private:
	GameObject* mCamera;	
	Texture* mRenderTex;
	Texture* mDepThTex;	
	Texture* mRWTex;
	Texture* RWTex2;
	std::wstring mLabel;
	FLOAT mBackgroundColor[4];
};

