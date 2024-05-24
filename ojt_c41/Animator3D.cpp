#include "pch.h"
#include "Animator3D.h"
#include "StructuredBuffer.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Anim3DBuuferCopyCS.h"
#include "ResourceManager.h"
#include "Engine.h"
#include "GraphicDeviceDx11.h"
#include "EnumShaderBindStage.h"
#include "GameObject.h"
#include "StructuredBuffer.h"
#include "TimeManager.h"
#include "Animation3DController.h"
#include "Transform.h"
//#include <Helper\FileHelper.h>

Animator3D::Animator3D()
	: Component(eComponentType::Animator3D)
	, mBones()
	, mCurAnimationFrame{}
	, mNextAnimationFrame{}
	, mMixRatio(0)
	, mbMix(false)
	, mSBBoneFinalBuffer(nullptr)
	, mAnimMatrixCS(nullptr)
	, mController(nullptr)
{
	mAnimMatrixCS =
		static_cast<Anim3DBuuferCopyCS*>(gResourceManager->Find<ComputeShader>(L"Animation3DCS"));
	mSBBoneFinalBuffer = new StructuredBuffer(16, 1, nullptr);
}

Animator3D::~Animator3D()
{
	DELETE_POINTER(mSBBoneFinalBuffer);
}

Animator3D::Animator3D(const Animator3D& other)
	: Component(eComponentType::Animator3D)
{
	(void)other;
}

void Animator3D::initialize()
{
	Animation3DController* controller = GetOwner()->FindParentByComponent<Animation3DController>();

	bool flag = true;
	for (Animator3D* animator3D : controller->mAnimation3Ds)
	{
		if (this == animator3D)
		{
			flag = false;
			break;
		}
	}

	if (flag)
	{
		controller->AddAnimator3D(this);
	}
}

void Animator3D::update()
{
}

//FIXME ComputeShader연산 항상하는것 고치기
void Animator3D::lateUpdate()
{
	// 컴퓨트 쉐이더 연산여부
	//m_bFinalMatUpdate = false;
}

void Animator3D::Play(const std::wstring& animationName)
{
	(void)animationName;
}

void Animator3D::Play(const std::wstring& animationName, const float ratio)
{
	(void)animationName;
	(void)ratio;
}


void Animator3D::UpdateData()
{
	Mesh* ppMesh = GetComponent<MeshRenderer>()->GetMesh();
	(void)ppMesh;
	if (true && !mBones.empty())
	{
		Mesh* pMesh = GetComponent<MeshRenderer>()->GetMesh();

		const UINT BONE_COUNT = GetBoneCount();
		if (mSBBoneFinalBuffer->GetElementCount() != BONE_COUNT)
		{
			DELETE_POINTER(mSBBoneFinalBuffer);
			mSBBoneFinalBuffer = new StructuredBuffer(sizeof(Matrix), BONE_COUNT, nullptr);
		}

		mAnimMatrixCS->SetFrameDataBuffer(pMesh->GetBoneFrameDataBuffer());
		mAnimMatrixCS->SetOffsetMatBuffer(pMesh->GetBoneOffsetBuffer());

		if (mbMix)
		{
			mAnimMatrixCS->SetMixFrameData(BONE_COUNT, mCurAnimationFrame.mFrameIdx, mCurAnimationFrame.mNextFrameIdx, mCurAnimationFrame.mRatio,
				mNextAnimationFrame.mFrameIdx, mNextAnimationFrame.mNextFrameIdx,
				mNextAnimationFrame.mRatio, mMixRatio);			
		}
		else
		{
			mAnimMatrixCS->SetFrameData(BONE_COUNT, mCurAnimationFrame.mFrameIdx, mCurAnimationFrame.mNextFrameIdx, mCurAnimationFrame.mRatio);
		}		
		mAnimMatrixCS->SetOutputBuffer(mSBBoneFinalBuffer);
		mAnimMatrixCS->UpdateData();

		gGraphicDevice->BindCS(mAnimMatrixCS);
		gGraphicDevice->Distpatch(mAnimMatrixCS);

		mAnimMatrixCS->Clear();
		mCurAnimationFrame.m_bFinalMatUpdate = true;
	}

	gGraphicDevice->BindSB(30, mSBBoneFinalBuffer, eShaderBindType::VS);
}

void Animator3D::ClearData()
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	UINT recentNum = 30;

	gGraphicDevice->UnSafe_Context()->VSSetShaderResources(recentNum, 1, &pSRV);
	gGraphicDevice->UnSafe_Context()->HSSetShaderResources(recentNum, 1, &pSRV);
	gGraphicDevice->UnSafe_Context()->DSSetShaderResources(recentNum, 1, &pSRV);
	gGraphicDevice->UnSafe_Context()->GSSetShaderResources(recentNum, 1, &pSRV);
	gGraphicDevice->UnSafe_Context()->PSSetShaderResources(recentNum, 1, &pSRV);
	gGraphicDevice->UnSafe_Context()->CSSetShaderResources(recentNum, 1, &pSRV);

	UINT iMtrlCount = GetComponent<MeshRenderer>()->GetMaterialCount();
	Material* pMtrl = nullptr;
	for (UINT i = 0; i < iMtrlCount; ++i)
	{
		pMtrl = GetComponent<MeshRenderer>()->GetMaterial(i);
		if (nullptr == pMtrl)
		{
			continue;
		}

		pMtrl->SetAnim3D(false); // Animation Mesh 알리기
		pMtrl->SetBoneCount(0);
	}
}

Matrix Animator3D::GetCurGrameBoneMatrix(int boneIdx)
{
	Matrix worldMatrix = GetComponent<Transform>()->GetWorldMatrix();
	float ratio = mCurAnimationFrame.mRatio;
	Assert(boneIdx < mBones.size(), ASSERT_MSG_INVALID);
	const tMTBone& BONE = mBones[boneIdx];
	const tMTKeyFrame& KEY = BONE.vecKeyFrame[mCurAnimationFrame.mFrameIdx];
	const tMTKeyFrame& NEXT_KEY = BONE.vecKeyFrame[(mCurAnimationFrame.mFrameIdx + 1) % BONE.vecKeyFrame.size()];

	Quaternion qRot = Quaternion::Slerp(Quaternion(KEY.qRot), Quaternion(NEXT_KEY.qRot), ratio);
	Vector3 pos = Vector3::Lerp(KEY.vTranslate, NEXT_KEY.vTranslate, ratio);
	Vector3 scale = Vector3::Lerp(KEY.vScale, NEXT_KEY.vScale, ratio);

	if (mbMix)
	{
		float ratio2 = mNextAnimationFrame.mRatio;
		const tMTKeyFrame& KEY_2= BONE.vecKeyFrame[mNextAnimationFrame.mFrameIdx];
		const tMTKeyFrame& NEXT_KEY_2 = BONE.vecKeyFrame[(mNextAnimationFrame.mFrameIdx + 1) % BONE.vecKeyFrame.size()];

		Quaternion qRot2 = Quaternion::Slerp(Quaternion(KEY_2.qRot), Quaternion(NEXT_KEY_2.qRot), ratio2);
		Vector3 pos2 = Vector3::Lerp(KEY_2.vTranslate, NEXT_KEY_2.vTranslate, ratio2);
		Vector3 scale2 = Vector3::Lerp(KEY_2.vScale, NEXT_KEY_2.vScale, ratio2);

		qRot = Quaternion::Slerp(qRot, qRot2, mMixRatio);
		pos = Vector3::Lerp(pos, pos2, mMixRatio);
		scale = Vector3::Lerp(scale, scale2, mMixRatio);
	}

	Matrix scaleMatrix = Matrix::CreateScale(scale);
	Matrix rotationMatrix = XMMatrixRotationQuaternion(qRot);
	Matrix transMatrix = Matrix::CreateTranslation(pos);
	worldMatrix = scaleMatrix * rotationMatrix * transMatrix * worldMatrix;
	return worldMatrix;
}

void Animator3D::Save(FILE* const file)
{
	Component::Save(file);

	//Bone 저장
	UINT boneCount = static_cast<UINT>(mBones.size());
	fwrite(&boneCount, sizeof(boneCount), 1, file);

	for (UINT i = 0; i < boneCount; ++i)
	{
		const tMTBone& bone = mBones[i];
		SaveWString(bone.strBoneName, file);
		fwrite(&bone.iDepth, sizeof(bone.iDepth), 1, file);
		fwrite(&bone.iParentIndx, sizeof(bone.iParentIndx), 1, file);
		fwrite(&bone.matOffset, sizeof(bone.matOffset), 1, file);
		fwrite(&bone.matBone, sizeof(bone.matBone), 1, file);

		UINT keyFrameCount = static_cast<UINT>(bone.vecKeyFrame.size());
		fwrite(&keyFrameCount, sizeof(keyFrameCount), 1, file);

		for (UINT j = 0; j < keyFrameCount; ++j)
		{
			const tMTKeyFrame& keyFrame = bone.vecKeyFrame[j];
			fwrite(&keyFrame.dTime, sizeof(keyFrame.dTime), 1, file);
			fwrite(&keyFrame.iFrame, sizeof(keyFrame.iFrame), 1, file);
			fwrite(&keyFrame.vTranslate, sizeof(keyFrame.vTranslate), 1, file);
			fwrite(&keyFrame.vScale, sizeof(keyFrame.vScale), 1, file);
			fwrite(&keyFrame.qRot, sizeof(keyFrame.qRot), 1, file);
		}

	}
}

void Animator3D::Load(FILE* const file)
{
	Component::Load(file);
	mBones.clear();

	//Bone Load
	UINT boneCount = 0;
	fread(&boneCount, sizeof(boneCount), 1, file);

	for (UINT i = 0; i < boneCount; ++i)
	{
		tMTBone bone = {};
		LoadWString(&bone.strBoneName, file);
		fread(&bone.iDepth, sizeof(bone.iDepth), 1, file);
		fread(&bone.iParentIndx, sizeof(bone.iParentIndx), 1, file);
		fread(&bone.matOffset, sizeof(bone.matOffset), 1, file);
		fread(&bone.matBone, sizeof(bone.matBone), 1, file);

		UINT keyFrameCount = 0;
		fread(&keyFrameCount, sizeof(keyFrameCount), 1, file);

		for (UINT j = 0; j < keyFrameCount; ++j)
		{
			tMTKeyFrame keyFrame = {};
			fread(&keyFrame.dTime, sizeof(keyFrame.dTime), 1, file);
			fread(&keyFrame.iFrame, sizeof(keyFrame.iFrame), 1, file);
			fread(&keyFrame.vTranslate, sizeof(keyFrame.vTranslate), 1, file);
			fread(&keyFrame.vScale, sizeof(keyFrame.vScale), 1, file);
			fread(&keyFrame.qRot, sizeof(keyFrame.qRot), 1, file);

			bone.vecKeyFrame.push_back(keyFrame);
		}

		mBones.push_back(bone);
	}


}
