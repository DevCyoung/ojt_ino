#include "pch.h"
#include "Rigidbody2D.h"
#include "Transform.h"
#include "TimeManager.h"

Rigidbody2D::Rigidbody2D()
	: Component(eComponentType::Rigidbody2D)
	, mForce(0.f)
	, mFriction(580.f)
	, mFrictionScale(1.3f)
	, mVelocityLimit(800.f)
	, mGravityLimit(700.f)
	, mGravityAccel(50000.f)
	, mbGravityUse(false)
	, mbGround(false)
	, mMass(1.f)	
	, mbFixedUpdate(true)
	, mbAirFriction(true)
{
}

Rigidbody2D::~Rigidbody2D()
{
}

void Rigidbody2D::initialize()
{
}

void Rigidbody2D::update()
{
}

void Rigidbody2D::fixedUpdate()
{
	// F = M x A
	// 가속도 구하기

	if (false == mbFixedUpdate)
	{
		return;
	}

	Vector2 vAccel = mForce / mMass;

	// 가속도를 이용해서 속도를 증가시킴
	float deltaTime = gDeltaTime;
	mVelocity += vAccel * deltaTime;

	// 중력을 사용하는 경우는, 땅에 있어야 마찰력 적용
	// 중력을 쓰지 않으면 마찰력 바로 적용
	if ((mbGravityUse && mbGround) || mbGravityUse == false)
	{
		// 마찰 가속도
		Vector2 vFriction = -mVelocity;
		if (vFriction != Vector2::Zero)
		{
			vFriction.Normalize();
			vFriction *= (mFriction * mFrictionScale * mMass * gDeltaTime);
		}

		// 속도 감소량(마찰력에 의한) 이 기존 속도를 넘어 설 경우, 완전 제로로 만든다.
		if (mVelocity.Length() <= vFriction.Length())
		{
			mVelocity = Vector2::Zero;
		}
		else
		{
			// 현재 속도 반대방향으로 마찰의 의한 속도 감소
			mVelocity += vFriction;
		}
	}

	//공기저항 X
	if (mbGravityUse && !mbGround && mbAirFriction)
	{		
		// 마찰 가속도
		Vector2 vFriction = -mVelocity;

		vFriction.y = 0.f;
		vFriction.Normalize();

		//공기저항계수
		vFriction *= 300.f * gDeltaTime;

		if (mVelocity.Length() <= vFriction.Length())
		{
			mVelocity.x = 0.f;
		}
		else
		{
			mVelocity += vFriction;
		}


		


		//if (vFriction != Vector2::Zero)
		//{
		//	vFriction.Normalize();
		//	vFriction *= (mFriction * mFrictionScale * mMass * gDeltaTime);
		//}

		//// 속도 감소량(마찰력에 의한) 이 기존 속도를 넘어 설 경우, 완전 제로로 만든다.
		//if (mVelocity.Length() <= vFriction.Length())
		//{
		//	mVelocity = Vector2::Zero;
		//}
		//else
		//{
		//	// 현재 속도 반대방향으로 마찰의 의한 속도 감소
		//	mVelocity += vFriction;
		//}
	}

	// 중력 적용 + 공중 상태 ==> 중력
	if (mbGravityUse && mbGround == false)
	{
		Vector2 vGravityAccel = Vector2(0.f, mGravityAccel);
		mVelocity -= vGravityAccel * gDeltaTime;
	}


	// 속도 제한 설정(좌, 우)
	// 중력을 사용하는 경우, 좌우 이동과 중력방향(아래) 에 속도제한을 따로 설정한다.
	if (mbGravityUse)
	{
		if (mVelocityLimit < fabsf(mVelocity.x))
		{
			mVelocity.x = (mVelocity.x / fabsf(mVelocity.x)) * mVelocityLimit;
		}

		if (mGravityLimit < fabsf(mVelocity.y))
		{
			mVelocity.y = (mVelocity.y / fabsf(mVelocity.y)) * mGravityLimit;
		}
	}

	// 중력을 사용하지 않으면, 어느 방향으로든 속도 제한을 건다.
	else
	{
		if (mVelocityLimit < mVelocity.Length())
		{
			// 제한 속도를 넘겼을 경우
			mVelocity.Normalize();
			mVelocity *= mVelocityLimit;
		}
	}

	// 오브젝트의 위치
	Vector3 Pos = GetOwner()->GetComponent<Transform>()->GetPosition();

	// 속도(속력 + 방향) 에 맞게 물체를 이동시킴
	Pos.x += mVelocity.x * gDeltaTime;
	Pos.y += mVelocity.y * gDeltaTime;

	GetOwner()->GetComponent<Transform>()->SetPosition(Pos);

	mForce = Vector2::Zero;
}

void Rigidbody2D::lateUpdate()
{

}

