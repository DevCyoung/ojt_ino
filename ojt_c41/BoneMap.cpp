#include "pch.h"
#include "BoneMap.h"
#include "Animator3D.h"
#include "Transform.h"

BoneMap::BoneMap()
	: Component(eComponentType::BoneMap)
	, mAnimator3D(nullptr)
	, mBonIdx(-1)
{
}

BoneMap::~BoneMap()
{
}

void BoneMap::update()
{
}

void BoneMap::lastUpdate()
{
	//현재프레임계산
	Assert(mBonIdx != -1, ASSERT_MSG_INVALID);
	Matrix mat = mAnimator3D->GetCurGrameBoneMatrix(mBonIdx);
	Matrix wordMatrix =  GetComponent<Transform>()->GetWorldMatrix();

	wordMatrix = wordMatrix * mat;
	GetComponent<Transform>()->SetWorldMatrix(wordMatrix);
}
