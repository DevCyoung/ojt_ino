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
	// ���ӵ� ���ϱ�

	if (false == mbFixedUpdate)
	{
		return;
	}

	Vector2 vAccel = mForce / mMass;

	// ���ӵ��� �̿��ؼ� �ӵ��� ������Ŵ
	float deltaTime = gDeltaTime;
	mVelocity += vAccel * deltaTime;

	// �߷��� ����ϴ� ����, ���� �־�� ������ ����
	// �߷��� ���� ������ ������ �ٷ� ����
	if ((mbGravityUse && mbGround) || mbGravityUse == false)
	{
		// ���� ���ӵ�
		Vector2 vFriction = -mVelocity;
		if (vFriction != Vector2::Zero)
		{
			vFriction.Normalize();
			vFriction *= (mFriction * mFrictionScale * mMass * gDeltaTime);
		}

		// �ӵ� ���ҷ�(�����¿� ����) �� ���� �ӵ��� �Ѿ� �� ���, ���� ���η� �����.
		if (mVelocity.Length() <= vFriction.Length())
		{
			mVelocity = Vector2::Zero;
		}
		else
		{
			// ���� �ӵ� �ݴ�������� ������ ���� �ӵ� ����
			mVelocity += vFriction;
		}
	}

	//�������� X
	if (mbGravityUse && !mbGround && mbAirFriction)
	{		
		// ���� ���ӵ�
		Vector2 vFriction = -mVelocity;

		vFriction.y = 0.f;
		vFriction.Normalize();

		//�������װ��
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

		//// �ӵ� ���ҷ�(�����¿� ����) �� ���� �ӵ��� �Ѿ� �� ���, ���� ���η� �����.
		//if (mVelocity.Length() <= vFriction.Length())
		//{
		//	mVelocity = Vector2::Zero;
		//}
		//else
		//{
		//	// ���� �ӵ� �ݴ�������� ������ ���� �ӵ� ����
		//	mVelocity += vFriction;
		//}
	}

	// �߷� ���� + ���� ���� ==> �߷�
	if (mbGravityUse && mbGround == false)
	{
		Vector2 vGravityAccel = Vector2(0.f, mGravityAccel);
		mVelocity -= vGravityAccel * gDeltaTime;
	}


	// �ӵ� ���� ����(��, ��)
	// �߷��� ����ϴ� ���, �¿� �̵��� �߷¹���(�Ʒ�) �� �ӵ������� ���� �����Ѵ�.
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

	// �߷��� ������� ������, ��� �������ε� �ӵ� ������ �Ǵ�.
	else
	{
		if (mVelocityLimit < mVelocity.Length())
		{
			// ���� �ӵ��� �Ѱ��� ���
			mVelocity.Normalize();
			mVelocity *= mVelocityLimit;
		}
	}

	// ������Ʈ�� ��ġ
	Vector3 Pos = GetOwner()->GetComponent<Transform>()->GetPosition();

	// �ӵ�(�ӷ� + ����) �� �°� ��ü�� �̵���Ŵ
	Pos.x += mVelocity.x * gDeltaTime;
	Pos.y += mVelocity.y * gDeltaTime;

	GetOwner()->GetComponent<Transform>()->SetPosition(Pos);

	mForce = Vector2::Zero;
}

void Rigidbody2D::lateUpdate()
{

}

