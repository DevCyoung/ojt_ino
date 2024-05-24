#include "pch.h"
#include "Transform.h"
#include "EngineMath.h"
#include "GameObject.h"

Transform::Transform()
	: Component(eComponentType::Transform)
	, mPosition(Vector3::Zero)
	, mRotation(Vector3::Zero)
	, mScale(Vector3::One)
	, mForward(Vector3::Forward)
	, mRight(Vector3::Right)
	, mUp(Vector3::Up)	
	, mWorld(Matrix::Identity)
{
}

Transform::~Transform()
{
}

Transform::Transform(const Transform& other)
	: Component(other)
	, mPosition(other.mPosition)
	, mRotation(other.mRotation)
	, mScale(other.mScale)
	, mForward(Vector3::Zero)
	, mRight(Vector3::Zero)
	, mUp(Vector3::Zero)
	, mWorld(Matrix::Identity)
{
}

void Transform::initialize()
{
}

void Transform::update()
{
}

void Transform::lateUpdate()
{
	CalculateTransform();
}
void Transform::CalculateTransform()
{
	mWorld = CreateWorldMatrix(mPosition, mRotation, mScale);

	GameObject* const parentOrNull = GetOwner()->GetParentOrNull();

	if (parentOrNull)
	{
		const Transform* const transform = parentOrNull->GetComponent<Transform>();
		mWorld *= transform->mWorld;
	}

	mUp = XMVector3TransformNormal(Vector3::Up, mWorld);
	mForward = XMVector3TransformNormal(Vector3::Forward, mWorld);
	mRight = XMVector3TransformNormal(Vector3::Right, mWorld);

	mUp.Normalize();
	mForward.Normalize();
	mRight.Normalize();
}

//FIX ME
void Transform::CalculateTransform(const Matrix& rotationMatrix)
{
	mWorld = CreateWorldMatrix(mPosition, rotationMatrix, mScale);

	GameObject* const parentOrNull = GetOwner()->GetParentOrNull();

	if (parentOrNull)
	{
		const Transform* const transform = parentOrNull->GetComponent<Transform>();
		mWorld *= transform->mWorld;
	}

	mUp = XMVector3TransformNormal(Vector3::Up, mWorld);
	mForward = XMVector3TransformNormal(Vector3::Forward, mWorld);
	mRight = XMVector3TransformNormal(Vector3::Right, mWorld);

	mUp.Normalize();
	mForward.Normalize();
	mRight.Normalize();
}

Matrix Transform::CreateWorldMatrix(const Vector3& position, const Vector3& rotation, const Vector3& scale)
{
	Matrix world = Matrix::Identity;

	Matrix scaleMatrix = Matrix::CreateScale(scale);

	Matrix rotationMatrix = {};
	rotationMatrix  = Matrix::CreateRotationX(Deg2Rad(rotation.x));
	rotationMatrix *= Matrix::CreateRotationY(Deg2Rad(rotation.y));
	rotationMatrix *= Matrix::CreateRotationZ(Deg2Rad(rotation.z));

	Matrix positionMatrix = {};
	positionMatrix.Translation(position);

	world *= scaleMatrix;
	world *= rotationMatrix;
	world *= positionMatrix;

	return world;
}

Matrix Transform::CreateWorldMatrix(const Vector3& position, const Matrix& rotationMatrix, const Vector3& scale)
{
	Matrix world = Matrix::Identity;

	Matrix scaleMatrix = Matrix::CreateScale(scale);

	Matrix positionMatrix = {};
	positionMatrix.Translation(position);

	world *= scaleMatrix;
	world *= rotationMatrix;
	world *= positionMatrix;

	return world;
}

Matrix Transform::GetTransformMatrix()
{
	Matrix positionMatrix = {};
	positionMatrix.Translation(mPosition);
	return positionMatrix;
}

Matrix Transform::GetRotationMatrix()
{
	Matrix rotationMatrix = {};
	rotationMatrix = Matrix::CreateRotationX(Deg2Rad(mRotation.x));
	rotationMatrix *= Matrix::CreateRotationY(Deg2Rad(mRotation.y));
	rotationMatrix *= Matrix::CreateRotationZ(Deg2Rad(mRotation.z));
	return rotationMatrix;
}

Matrix Transform::GetScaleMatrix()
{
	Matrix scaleMatrix = Matrix::CreateScale(mScale);
	return scaleMatrix;
}

void Transform::SetFlipx(const bool bFlipx)
{
	if (bFlipx)
	{
		mRotation.y = 180.f;
	}
	else
	{
		mRotation.y = 0.f;
	}
}

bool Transform::GetFlipX() const
{
	bool bFlipx = false;

	if (mRotation.y != 0.f)
	{
		bFlipx = true;
	}

	return bFlipx;
}

void Transform::Save(FILE* const file)
{
	Component::Save(file);
	fwrite(&mPosition,	sizeof(Vector3), 1, file);
	fwrite(&mRotation,	sizeof(Vector3), 1, file);
	fwrite(&mScale,		sizeof(Vector3), 1, file);
}

void Transform::Load(FILE* const file)
{	
	Component::Load(file);
	fread(&mPosition,	sizeof(Vector3), 1, file);
	fread(&mRotation,	sizeof(Vector3), 1, file);
	fread(&mScale,		sizeof(Vector3), 1, file);
}
