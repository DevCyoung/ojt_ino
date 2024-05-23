#pragma once
#include "Component.h"
#include "Animator3D.h"
REGISTER_COMPONENT_TYPE(Animation3DController);

//class Animator3D;

class Animation3DController : public Component
{
public:
	Animation3DController();
	virtual ~Animation3DController();
	//Animation3DController(const Animation3DController&);
	Animation3DController& operator=(const Animation3DController&) = delete;

	void AddAnimator3D(Animator3D* animator3D)
	{
		animator3D->SetController(this);
		mAnimation3Ds.push_back(animator3D);
	}

	void Play(const std::wstring& animation);
	void Play(const std::wstring& animation, float ratio);

	const std::vector<Animator3D*>& GetAnimation3Ds() { return mAnimation3Ds; };
	virtual void update();
	virtual void lateUpdate();

	//void	SetBones(std::vector<tMTBone>* bones) { mBones = bones; }
	void	SetClips(std::vector<tMTAnimClip> clip) { mAnimClips = clip; }
	void	SetClip(int clipIdx) { mCurClip = clipIdx; }
	void	Stop(bool stop) { mbStop = stop; }
	bool	IsStop() { return mbStop; }
	int		GetCurFrameIdx() { return mCurFrameIdx; }
	void	SetCurFrameIde(int frameIdx) { mCurFrameIdx = frameIdx; }
	void	SetAnimClip(std::vector<tMTAnimClip> _vecAnimClip);
	int		GetAnimationClipIdx(const std::wstring& name);
	void	RemoveClip(const std::wstring& clipName);
	void	CreateClip(const std::wstring& clipName, int startFrame, int endFrame);

	void	GetFrameData(int clip, int* outFrame, int* outNextFrame, float* outRatio);

	virtual void Save(FILE* const file) override;
	virtual void Load(FILE* const file) override;

	std::vector<tMTAnimClip>& GetAnimationClip() { return mAnimClips; }

	std::vector<Animator3D*>	mAnimation3Ds;
	std::vector<tMTAnimClip>	mAnimClips;
	int							mFramePer;
	bool						mbStop;	

	int							mCurClip;
	int							mCurFrameIdx;
	int							mCurNextFrameIdx;	
	float						mCurRatio;

	bool						mbOtherClip;

	int							mOtherClip;
	int							mOtherFrameIdx;
	int							mOtherNextFrameIdx;
	float						mOtherRatio;

	float						mCurBlendTime;
	float						mMixBlendTime;


	std::vector<float>			m_vecClipUpdateTime;


	CLONE(Animation3DController);
};

