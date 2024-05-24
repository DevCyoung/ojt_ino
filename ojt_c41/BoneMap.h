#pragma once
#include "Component.h"

REGISTER_COMPONENT_TYPE(BoneMap);
class Animator3D;

class BoneMap : public Component
{
public:
	BoneMap();
	virtual ~BoneMap();
	BoneMap(const BoneMap&) = delete;
	BoneMap& operator=(const BoneMap&) = delete;
	void SetAnimator3D(Animator3D* animator3d, int bonIdx) { mAnimator3D = animator3d; mBonIdx = bonIdx; }

	virtual void update() override final;
	virtual void lastUpdate() override final;

	int mBonIdx;
	Animator3D* mAnimator3D;	
};

