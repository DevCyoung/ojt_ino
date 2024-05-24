#pragma once
#include "Component.h"

enum class eCollider2DType
{
	Box = 1,
	Circle = 2,
	Line = 4,
	BoxCircle = Box | Circle,
};

REGISTER_COMPONENT_TYPE(Collider2D);


class Collider2D : public Component
{
	friend class CollisionManagement2D;
public:
	Collider2D();
	virtual ~Collider2D();
	Collider2D(const eComponentType componentType,
		const eCollider2DType mColllider2DType);
	Collider2D(const Collider2D&);
	Collider2D& operator=(const Collider2D&) = delete;

	eCollider2DType GetCollideType() const { return mColllider2DType; }
	const Matrix& GetColliderWorldMatrix() const { return mColliderWorldMat; }

	const Vector3& GetSize() const { return mSize; }
	const Vector3& GetOffset() const { return mOffset; }
	float GetRadius() const { return mRadius; }

	Vector3 GetStartPos() const { return mStartPos; }
	Vector3 GetEndPos() const { return mEndPos; }

	void SetSize(const Vector2& size) { mSize = Vector3(size.x, size.y, 1.f); }
	void SetSize(const float x, float y) { mSize.x = x; mSize.y = y; mSize.z = 1.f; }
	void SetOffset(const Vector2& offset) { mOffset = Vector3(offset.x, offset.y, 0.f); }
	void SetRadius(const float radius) { mRadius = radius; }
	void SetPoints(float sx, float sy, float ex, float ey)
	{
		mStartPos = Vector3(sx, sy, 0.f);
		mEndPos = Vector3(ex, ey, 0.f);
	}	

	void TurnOnVisible() { mbVisible = true; }
	void TurnOffVisible() { mbVisible = false; }

	void SetColor(const Vector4& color) { mColor = color; }	

	CLONE(Collider2D)
private:
	virtual void initialize() override;
	virtual void update() override;
	virtual void lateUpdate() override;

	void onCollisionEnter(Collider2D* other);
	void onCollisionStay(Collider2D* other);
	void onCollisionExit(Collider2D* other);



protected:
	eCollider2DType mColllider2DType;
	Matrix mColliderWorldMat;

	//Rect
	Vector3 mSize;

	Vector3 mOffset;
	UINT mCollisionCount;

	//Circle
	float mRadius;

	//Line
	Vector3 mStartPos;
	Vector3 mEndPos;

	Vector4 mColor;

	bool mbVisible;
};
