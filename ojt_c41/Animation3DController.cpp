#include "pch.h"
#include "Animation3DController.h"
#include "Animator3D.h"
#include "TimeManager.h"
//#include <Helper/FileHelper.h>
//#include <Helper/StringHelper.h>
//WALK_A  134  223
//WALK_B  225  314
//Attack1  498  619

Animation3DController::Animation3DController()
	: Component(eComponentType::Animation3DController)
	, mFramePer(30)
	, mbStop(false)
	, mCurClip(0)
	, mCurFrameIdx(0)
	, mCurNextFrameIdx(0)
	, mCurRatio(0)	
	, mbOtherClip(false)
	, mOtherClip(0)
	, mOtherFrameIdx(0)
	, mOtherNextFrameIdx(0)
	, mOtherRatio(0)
	, mCurBlendTime(0)
	, mMixBlendTime(0)
	, m_vecClipUpdateTime(0)
{
}

Animation3DController::~Animation3DController()
{
}

void Animation3DController::Play(const std::wstring& animation)
{
	for (Animator3D* animator3D : mAnimation3Ds)
	{
		animator3D->Play(animation);
	}
}

void Animation3DController::Play(const std::wstring& animation, float blendTime)
{
	//이전의 애니메이션과 블렌딩한다.
	int idx = GetAnimationClipIdx(animation);
	Assert(idx != -1, ASSERT_MSG_INVALID);

	mbOtherClip = true;
	mOtherClip = idx;
	m_vecClipUpdateTime[mOtherClip] = 0.f;
	mCurBlendTime = 0.f;
	mMixBlendTime = blendTime;
}

void Animation3DController::update()
{	
	// 현재 재생중인 Clip 의 시간을 진행한다.
	GetFrameData(mCurClip, &mCurFrameIdx, &mCurNextFrameIdx, &mCurRatio);

	if (mbOtherClip)
	{
		GetFrameData(mOtherClip, &mOtherFrameIdx, &mOtherNextFrameIdx, &mOtherRatio);
		mCurBlendTime += gDeltaTime;
	}

	//두개의 애니메이션을 진행한다.
	for (Animator3D* animator3D : mAnimation3Ds)
	{
		if (mbOtherClip)
		{
			float blendRatio = mCurBlendTime / mMixBlendTime;
			blendRatio = blendRatio > 1.f ? 1 : blendRatio;
			animator3D->setMixAnimationFrame(mCurFrameIdx, mCurNextFrameIdx, mCurRatio,
				mOtherFrameIdx, mOtherNextFrameIdx, mOtherRatio, blendRatio);
		}
		else
		{
			animator3D->setCurrentAnimationFrame(mCurFrameIdx, mCurNextFrameIdx, mCurRatio);
		}		
	}

	//Ratio끝나면 바꿈 선형적으로 섞는시간
	if (mbOtherClip && mCurBlendTime >= mMixBlendTime)
	{
		mCurClip = mOtherClip;
		mbOtherClip = false;
	}
}

void Animation3DController::RemoveClip(const std::wstring& clipName)
{
	if (mAnimClips.size() <= 1)
	{
		return;
	}

	std::vector<tMTAnimClip>::iterator iter = mAnimClips.begin();

	for (; iter != mAnimClips.end(); ++iter)
	{
		if (iter->strAnimName == clipName)
		{
			mAnimClips.erase(iter);
			break;
		}
	}

	mCurClip = 0;
}

void Animation3DController::CreateClip(const std::wstring& clipName, int startFrame, int endFrame)
{
	std::vector<tMTAnimClip>::iterator iter = mAnimClips.begin();

	for (; iter != mAnimClips.end(); ++iter)
	{
		if (iter->strAnimName == clipName)
		{			
			return;
		}
	}

	tMTAnimClip clip = {};
	clip.iStartFrame = startFrame;
	clip.iEndFrame = endFrame;
	clip.strAnimName = clipName;
	mAnimClips.push_back(clip);
	SetAnimClip(mAnimClips);
}

void Animation3DController::GetFrameData(int clip, 
	int* outFrame, 
	int* outNextFrame, 
	float* outRatio)
{
	Assert(outFrame, ASSERT_MSG_NULL);
	Assert(outNextFrame, ASSERT_MSG_NULL);
	Assert(outRatio, ASSERT_MSG_NULL);

	float curTime = 0.f;

	if (!mbStop && !m_vecClipUpdateTime.empty())
	{
		m_vecClipUpdateTime[clip] += gDeltaTime;
		int timelength = mAnimClips[clip].iEndFrame - mAnimClips[clip].iStartFrame;
		if (m_vecClipUpdateTime[clip] >= (float)timelength / mFramePer)
		{
			m_vecClipUpdateTime[clip] = 0.f;
		}


		float startTime = (float)mAnimClips[clip].iStartFrame / mFramePer;
		curTime = startTime + m_vecClipUpdateTime[clip];

		// 현재 프레임 인덱스 구하기
		double dFrameIdx = curTime * (double)mFramePer;
		*outFrame = (int)(dFrameIdx);

		// 다음 프레임 인덱스
		if (*outFrame >= mAnimClips[clip].iEndFrame)
		{
			*outNextFrame = mAnimClips[clip].iStartFrame;	// 끝이면 다음인덱스
		}
		else
		{
			*outNextFrame = *outFrame + 1;
		}

		// 프레임간의 시간에 따른 비율을 구해준다.
		*outRatio = (float)(dFrameIdx - (double)*outFrame);
	}
}

void Animation3DController::lateUpdate()
{
}

void Animation3DController::SetAnimClip(std::vector<tMTAnimClip> _vecAnimClip)
{
	mAnimClips = _vecAnimClip;
	m_vecClipUpdateTime.resize(mAnimClips.size());
}

int Animation3DController::GetAnimationClipIdx(const std::wstring& name)
{
	int idx = -1;
	for (int i = 0; i < mAnimClips.size(); ++i)
	{
		if (mAnimClips[i].strAnimName == name)
		{
			idx = i;
			break;
		}
	}

	return idx;
}

void Animation3DController::Save(FILE* const file)
{
	Component::Save(file);

	UINT animClipCount = static_cast<UINT>(mAnimClips.size());
	fwrite(&animClipCount, sizeof(animClipCount), 1, file);

	for (UINT i = 0; i < animClipCount; ++i)
	{
		const tMTAnimClip& clip = mAnimClips[i];
		SaveWString(clip.strAnimName, file);
		fwrite(&clip.iStartFrame, sizeof(clip.iStartFrame), 1, file);
		fwrite(&clip.iEndFrame, sizeof(clip.iEndFrame), 1, file);
		fwrite(&clip.iFrameLength, sizeof(clip.iFrameLength), 1, file);
		fwrite(&clip.dStartTime, sizeof(clip.dStartTime), 1, file);
		fwrite(&clip.dEndTime, sizeof(clip.dEndTime), 1, file);
		fwrite(&clip.dTimeLength, sizeof(clip.dTimeLength), 1, file);
		fwrite(&clip.fUpdateTime, sizeof(clip.fUpdateTime), 1, file);		
		fwrite(&clip.eMode, sizeof(clip.eMode), 1, file);
	}
}

void Animation3DController::Load(FILE* const file)
{
	Component::Load(file);

	UINT animClipCount = 0;
	fread(&animClipCount, sizeof(animClipCount), 1, file);

	mAnimClips.reserve(animClipCount);
	for (UINT i = 0; i < animClipCount; ++i)
	{
		tMTAnimClip clip = {};
		LoadWString(&clip.strAnimName, file);
		fread(&clip.iStartFrame, sizeof(clip.iStartFrame), 1, file);
		fread(&clip.iEndFrame, sizeof(clip.iEndFrame), 1, file);
		fread(&clip.iFrameLength, sizeof(clip.iFrameLength), 1, file);
		fread(&clip.dStartTime, sizeof(clip.dStartTime), 1, file);
		fread(&clip.dEndTime, sizeof(clip.dEndTime), 1, file);
		fread(&clip.dTimeLength, sizeof(clip.dTimeLength), 1, file);
		fread(&clip.fUpdateTime, sizeof(clip.fUpdateTime), 1, file);
		fread(&clip.eMode, sizeof(clip.eMode), 1, file);
		mAnimClips.push_back(clip);
	}

	m_vecClipUpdateTime.clear();
	m_vecClipUpdateTime.resize(animClipCount);
}