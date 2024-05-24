#pragma once
#include "RenderComponent.h"
#include "StructBuffer.h"

REGISTER_COMPONENT_TYPE(Animator2D);

class Texture;
class Animation2D;

class Animator2D : public RenderComponent
{
public:
	struct Event
	{
		void operator=(std::function<void()> func)
		{
			mEvent = std::move(func);
		}
		void operator()()
		{
			if (mEvent)
				mEvent();
		}

		std::function<void()> mEvent;
	};

	struct Events
	{
		Event startEvent;
		Event completeEvent;
		Event endEvent;
	};

public:
	Animator2D();
	virtual ~Animator2D();
	Animator2D(const Animator2D& other);
	Animator2D& operator=(const Animator2D&) = delete;

	void Play(const std::wstring& animKey, const bool bLoop);
	void PlayFrame(const std::wstring& animKey, const UINT frame, const bool bLoop);

	void CreateAnimation(const std::wstring& animName,
		Texture* const atlas,	
		const UINT frameCount,
		const XMUINT2 startLeft,
		const XMUINT2 frameSize,		
		const XMUINT2 framePad,
		const XMINT2 frameoffset = XMINT2(0, 0),
		const float duration = 0.f);

	tCBTransform GetCBTransform() const;
	tCBAnimationInfo GetCBAnimationInfo() const;	

	Animation2D* FindAnimationOrNull(const std::wstring& animName) const;	
	Animation2D* GetCurAnimationOrNull() const { return mCurAnimation; }

	void SetBackSize(const XMUINT2& size) { backSize = size; }	
	void SetFlipX(const bool bFlip) { bFlipX = bFlip; }
	void SetColorInfo(const tCBColorInfo& colorInfo) { mColorInfo = colorInfo; }

	Events* FindEvents(const std::wstring animName) const;
	const std::function<void()>& FindStartEvent(const std::wstring animName);
	const std::function<void()>& FindCompleteEvent(const std::wstring animName);
	const std::function<void()>& FindEndEvent(const std::wstring animName);


	void SetColorR(float r);
	void SetColorG(float g);
	void SetColorB(float b);
	void SetColorA(float a);
	void SetColorReset();

	void MulColorR(float r);
	void MulColorG(float g);
	void MulColorB(float b);
	void MulColorA(float a);
	void MulColor(float r, float g, float b, float a);
	void MulColorReset();

	CLONE(Animator2D)
private:
	virtual void initialize() override final;
	virtual void update() override final;
	virtual void lateUpdate() override final;
	virtual void render(const Matrix& viewMatrix, const Matrix& projectionMatrix) override final;
	
	tCBColorInfo mColorInfo;

	std::unordered_map<std::wstring , Animation2D*>  mAnimationMap;
	std::unordered_map<std::wstring, Events*> mEvents;
	Animation2D* mCurAnimation;
	bool bRepeat;
	bool bFlipX;
	XMUINT2 backSize;	
};
