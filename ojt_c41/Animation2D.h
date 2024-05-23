#pragma once

class Texture;

class Animation2D
{
	friend class Animator2D;

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
		Event endEvent;
	};

	struct tFrame
	{
		Vector2 UVLeftTop;
		Vector2 UVSize;
		Vector2	UVOffset;
		XMINT2	center;
		float duration;
		Event endEvent;
		Event startEvent;
		bool bEndEvent;
		bool bStartEvent;
	};


public:
	Animation2D(const std::wstring& animName,
		Texture* const atlas,
		const UINT frameCount,
		const XMUINT2 startLeft,
		const XMUINT2 frameSize,		
		const XMUINT2 framePad,
		const XMINT2 frameoffset,
		const float duration);

	virtual ~Animation2D();
	Animation2D(const Animation2D&) = delete;
	Animation2D& operator=(const Animation2D&) = delete;

	const tFrame& GetCurFrame() const { return mFrames[mCurFrameIdx]; }
	Texture* GetAtlas() const { Assert(mAtlas, ASSERT_MSG_NULL); return mAtlas; }

	bool IsFinished() const { return mbFinished; }

	void SetFrameEndEvent(UINT frameIdx, std::function<void()> func)
	{
		Assert(frameIdx < mFrames.size(), ASSERT_MSG_INVALID);

		mFrames[frameIdx].endEvent = std::move(func);
		mFrames[frameIdx].bEndEvent = true;
	}

	void SetFrameStartEvent(UINT frameIdx, std::function<void()> func)
	{
		Assert(frameIdx < mFrames.size(), ASSERT_MSG_INVALID);

		mFrames[frameIdx].startEvent = std::move(func);
		mFrames[frameIdx].bStartEvent = true;
	}

	void SetFirstFrameEndEvent(std::function<void()> func)
	{
		Assert(!mFrames.empty(), ASSERT_MSG_INVALID);

		SetFrameEndEvent(0, std::move(func));
	}

	void SetLastFrameEndEvent(std::function<void()> func)
	{
		Assert(!mFrames.empty(), ASSERT_MSG_INVALID);

		SetFrameEndEvent(static_cast<int>(mFrames.size()) - 1, std::move(func));
	}

	const std::wstring& GetKey() const { return mKey; }
	UINT GetFrameIdx() const { return static_cast<UINT>(mCurFrameIdx); }

	//CLONE(Animation2D)
private:
	void lateUpdate();
	void reset();
	void reset(const UINT frameIdx);

private:
	std::wstring mKey;
	Texture* mAtlas;
	bool mbFinished;
	float mCurFramePlayTime;
	int mCurFrameIdx;	
	bool mbFrameStart;
	std::vector<tFrame> mFrames;	
};

