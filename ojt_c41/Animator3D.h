#pragma once
#include "Component.h"
#include "FBXLoader.h"

REGISTER_COMPONENT_TYPE(Animator3D);

class StructuredBuffer;
class Mesh;
class Anim3DBuuferCopyCS;
class Animation3DController;

struct tFrameData
{
    int							    mFrameIdx;              // 클립의 현재 프레임
    int							    mNextFrameIdx;          // 클립의 다음 프레임
    float						    mRatio;	                // 프레임 사이 비율    
    bool						    m_bFinalMatUpdate;      // 최종행렬 연산 수행여부
};

class Animator3D : public Component
{

public:
    friend class Animation3DController;
	Animator3D();
	virtual ~Animator3D();
	Animator3D(const Animator3D& other);
	Animator3D& operator=(const Animator3D&) = delete;

    void UpdateData();

public:
    void SetBones(std::vector<tMTBone> _vecBones) { mBones = _vecBones; }
    StructuredBuffer* GetFinalBoneMat() { return mSBBoneFinalBuffer; }
    UINT GetBoneCount() { return static_cast<UINT>(mBones.size()); }

    void ClearData();
    virtual void initialize() override;
    virtual void update() override;
    virtual void lateUpdate() override;

    void Play(const std::wstring& animationName);
    void Play(const std::wstring& animationName, const float ratio);
    float GetCurAnimationRatio() { return mCurAnimationFrame.mRatio; }
    void SetController(Animation3DController* const controller) { mController = controller; }
    Animation3DController* GetController() { return mController; }
    Matrix GetCurGrameBoneMatrix(int boneIdx);
    const std::vector<tMTBone>& GetBones() { return mBones; }

    virtual void Save(FILE* const file) override;
    virtual void Load(FILE* const file) override;
private:
    void setCurrentAnimationFrame(int frameIde, int nextFrameIdx, float ratio)
    {
        mCurAnimationFrame.mFrameIdx = frameIde;
        mCurAnimationFrame.mNextFrameIdx = nextFrameIdx;
        mCurAnimationFrame.mRatio = ratio;

        mbMix = false;
    }

    void setMixAnimationFrame(int frame, int nextFrame, float ratio,
        int frame2, int nextFrame2, float ratio2, float mixRatio)
    {
        mCurAnimationFrame.mFrameIdx = frame;
        mCurAnimationFrame.mNextFrameIdx = nextFrame;
        mCurAnimationFrame.mRatio = ratio;

        mNextAnimationFrame.mFrameIdx = frame2;
        mNextAnimationFrame.mNextFrameIdx = nextFrame2;
        mNextAnimationFrame.mRatio = ratio2;

        mMixRatio = mixRatio;

        mbMix = true;
    }
public:
    std::vector<tMTBone>            mBones;        

    Anim3DBuuferCopyCS*             mAnimMatrixCS;

    tFrameData                      mCurAnimationFrame;
    tFrameData                      mNextAnimationFrame;
    float                           mMixRatio;
    bool                            mbMix;
    Animation3DController*          mController;
    StructuredBuffer*               mSBBoneFinalBuffer;     // 특정 프레임의 최종 행렬
};

