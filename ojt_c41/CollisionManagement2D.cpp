#include "pch.h"
#include "CollisionManagement2D.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "RectCollider2D.h"
#include "EngineMath.h"
#include "Transform.h"

/*
* Circle and Box Test
* http://jeffreythompson.org/collision-detection/circle-rect.php
* 
* checkCollision2DBoxAndBox
* Vector dot product = a * a' + b * b' + c * c' .....
* a(x, y, 0) b(x', y', z')
* dot => xx' + yy' + 0z'
* so, xx' + yy'
*/

CollisionManagement2D::CollisionManagement2D()
	: mCollisionMatrix{ 0, }
	, mCollisionCheckMap()
{
}

CollisionManagement2D::~CollisionManagement2D()
{
}

void CollisionManagement2D::TurnOnAllCollisionLayer()
{
	for (UINT& matrix : mCollisionMatrix)
	{
		matrix = 0XFFFFFFFF;
	}
}

void CollisionManagement2D::TurnOffAllCollisionLayer()
{
	for (UINT& matrix : mCollisionMatrix)
	{
		matrix = 0;
	}
}

void CollisionManagement2D::TurnOnCollisionLayer(const eLayerType left, const eLayerType right)
{	
	UINT row = static_cast<UINT>(left);
	UINT col = static_cast<UINT>(right);

	if (row > col)
	{
		UINT iTemp = col;
		col = row;
		row = iTemp;
	}

	mCollisionMatrix[row] |= (1 << col);
}

void CollisionManagement2D::TurnOffCollisionLayer(const eLayerType left, const eLayerType right)
{	
	UINT row = static_cast<UINT>(left);
	UINT col = static_cast<UINT>(right);

	if (row > col)
	{
		const UINT iTemp = col;
		col = row;
		row = iTemp;
	}

	mCollisionMatrix[row] &= ~(1 << col);
}

void CollisionManagement2D::phisicsUpdate(const Scene* const scene)
{
	for (UINT row = 0; row < static_cast<UINT>(eLayerType::End); ++row)
	{
		for (UINT col = row; col < static_cast<UINT>(eLayerType::End); ++col)
		{
			if (!(mCollisionMatrix[row] & (1 << col)))
			{
				continue;
			}

			const Layer& left = scene->mLayers[row];
			const Layer& right = scene->mLayers[col];

			const std::vector<GameObject*>& leftGameObjects = left.GetGameObjects();
			const std::vector<GameObject*>& rightGameObjects = right.GetGameObjects();

			if (row == col)
			{
				for (UINT i = 0; i < leftGameObjects.size(); ++i)
				{
					for (UINT j = i + 1; j < rightGameObjects.size(); ++j)
					{
						collide2DGameObject(leftGameObjects[i], rightGameObjects[j]);
					}
				}
			}
			else
			{
				for (UINT i = 0; i < leftGameObjects.size(); ++i)
				{
					for (UINT j = 0; j < rightGameObjects.size(); ++j)
					{
						collide2DGameObject(leftGameObjects[i], rightGameObjects[j]);
					}
				}
			}
		}
	}
}

void CollisionManagement2D::collide2DGameObject(const GameObject* const left,
	const GameObject* const right)
{
	Collider2D*  const leftCollider = left->GetComponentOrNull<Collider2D>();
	Collider2D*  const rightCollider = right->GetComponentOrNull<Collider2D>();

	if (leftCollider == nullptr || rightCollider == nullptr)
	{
		return;
	}

	CollisionID collisionID = {};

	collisionID.entityID.LeftID = leftCollider->GetID();
	collisionID.entityID.RightID = rightCollider->GetID();

	std::unordered_map<UINT_PTR, bool>::iterator iter
		= mCollisionCheckMap.find(collisionID.id);

	if (iter == mCollisionCheckMap.end())
	{
		mCollisionCheckMap.insert(std::make_pair(collisionID.id, false));
		iter = mCollisionCheckMap.find(collisionID.id);
	}

	bool bExit = false;

	if (left->GetState() == GameObject::eState::Destroy ||
		right->GetState() == GameObject::eState::Destroy)

	{
		bExit = true;
	}

	const eCollider2DType LEFT_COLLIDER_TYPE = leftCollider->GetCollideType();
	const eCollider2DType RIGHT_COLLIDER_TYPE = rightCollider->GetCollideType();

	bool bCollision = false;

	if (LEFT_COLLIDER_TYPE == RIGHT_COLLIDER_TYPE)
	{
		if (LEFT_COLLIDER_TYPE == eCollider2DType::Box)
		{
			bCollision = checkCollision2DBoxAndBox(leftCollider, rightCollider);
		}
		else if (LEFT_COLLIDER_TYPE == eCollider2DType::Circle)
		{
			bCollision = checkCollision2DCircleAndCircle(leftCollider, rightCollider);
		}
		else if (LEFT_COLLIDER_TYPE == eCollider2DType::Line)
		{
			bCollision = checkCollision2DLineAndLine(leftCollider, rightCollider);
		}
		else
		{
			Assert(false, ASSERT_MSG_INVALID);
		}
	}
	else
	{

		
		if (eCollider2DType::Box == LEFT_COLLIDER_TYPE && eCollider2DType::Circle == RIGHT_COLLIDER_TYPE)
		{
			bCollision = checkCollision2DBoxAndCircle(leftCollider, rightCollider);
		}
		else if (eCollider2DType::Box == RIGHT_COLLIDER_TYPE && LEFT_COLLIDER_TYPE == eCollider2DType::Circle)
		{
			bCollision = checkCollision2DBoxAndCircle(rightCollider, leftCollider);
		}
		else if (eCollider2DType::Box == RIGHT_COLLIDER_TYPE && LEFT_COLLIDER_TYPE == eCollider2DType::Line)
		{
			bCollision = checkCollision2DBoxAndLine(rightCollider, leftCollider);
		}
		else if (eCollider2DType::Box == LEFT_COLLIDER_TYPE && RIGHT_COLLIDER_TYPE == eCollider2DType::Line)
		{
			bCollision = checkCollision2DBoxAndLine(leftCollider, rightCollider);
		}
		else
		{
			//Assert(false, WCHAR_IS_INVALID_TYPE);
		}



	}

	if (bCollision)
	{
		if (bExit && iter->second)
		{
			leftCollider->onCollisionExit(rightCollider);
			rightCollider->onCollisionExit(leftCollider);
			iter->second = false;
		}
		else if (iter->second)
		{			
			leftCollider->onCollisionStay(rightCollider);
			rightCollider->onCollisionStay(leftCollider);
		}
		else
		{			
			if (bExit == false)
			{
				leftCollider->onCollisionEnter(rightCollider);
				rightCollider->onCollisionEnter(leftCollider);
				iter->second = true;
			}
		}
	}
	else
	{		
		if (iter->second)
		{
			leftCollider->onCollisionExit(rightCollider);
			rightCollider->onCollisionExit(leftCollider);
			iter->second = false;
		}
	}
}

bool CollisionManagement2D::checkCollision2DBoxAndBox(const Collider2D* const left, 
	const Collider2D* const right)
{
	Vector3 vertexPosArray[4] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
	};

	Vector3 sideDistArray[4] = {};

	sideDistArray[0] = XMVector3TransformNormal(vertexPosArray[1], left->GetColliderWorldMatrix()) -
		XMVector3TransformNormal(vertexPosArray[0], left->GetColliderWorldMatrix());
	sideDistArray[1] = XMVector3TransformNormal(vertexPosArray[3], left->GetColliderWorldMatrix()) -
		XMVector3TransformNormal(vertexPosArray[0], left->GetColliderWorldMatrix());

	sideDistArray[2] = XMVector3TransformNormal(vertexPosArray[1], right->GetColliderWorldMatrix()) -
		XMVector3TransformNormal(vertexPosArray[0], right->GetColliderWorldMatrix());
	sideDistArray[3] = XMVector3TransformNormal(vertexPosArray[3], right->GetColliderWorldMatrix()) -
		XMVector3TransformNormal(vertexPosArray[0], right->GetColliderWorldMatrix());

	const Vector3& DIST = XMVector3TransformCoord(Vector3::Zero, right->GetColliderWorldMatrix()) -
		XMVector3TransformCoord(Vector3::Zero, left->GetColliderWorldMatrix());

	for (int i = 0; i < 4; ++i)
	{
		Vector3 axis = sideDistArray[i];
		axis.Normalize();
		
		float axisProjDistSum = 0.f;
		for (int j = 0; j < 4; ++j)
		{						
			axisProjDistSum += fabsf(sideDistArray[j].Dot(axis));
		}
		axisProjDistSum /= 2.f;

		const float CENTER_PROJ_DIST = fabsf(DIST.Dot(axis));

		if (axisProjDistSum < CENTER_PROJ_DIST)
		{
			return false;
		}			
	}

	return true;
}

bool CollisionManagement2D::checkCollision2DCircleAndCircle(const Collider2D* const left, 
	const Collider2D* const right)
{		
	const float RADIUS_SUM = getCircleRadius(left) + getCircleRadius(right);
	const Vector3& DIST = left->GetColliderWorldMatrix().Translation() - 
		right->GetColliderWorldMatrix().Translation();

	return DIST.x * DIST.x + DIST.y * DIST.y < RADIUS_SUM * RADIUS_SUM;
}

bool CollisionManagement2D::checkCollision2DLineAndLine(const Collider2D* const left,
	const Collider2D* const right)
{
	Vector3 tp1 = left->GetColliderWorldMatrix().Translation();
	Vector3 tp2 = right->GetColliderWorldMatrix().Translation();

	Vector3 start = left->GetStartPos() + tp1;
	Vector3 end = left->GetEndPos() + tp1;

	Vector3 start2 = right->GetStartPos() + tp2;
	Vector3 end2 = right->GetEndPos() + tp2;

	Vector2 out = Vector2::Zero;

	return helper::math::LineAndLineCollision(start, end, start2, end2, &out);
}

bool CollisionManagement2D::checkCollision2DBoxAndLine(const Collider2D* const box, const Collider2D* const line)
{
	const Matrix& BOX_MAT = box->GetColliderWorldMatrix();
	const Matrix& LINE_MAT = line->GetColliderWorldMatrix();
	const Vector3& S = line->GetStartPos();
	const Vector3& E = line->GetEndPos();

	Vector2 out = Vector2::Zero;

	return helper::math::BoxAndLineCollision(BOX_MAT, LINE_MAT.Translation(), S, E, &out);
}

bool CollisionManagement2D::checkCollision2DBoxAndCircle(const Collider2D* const box, 
	const Collider2D* const circle)
{
	constexpr const Vector3 vertexPosArray[4] =
	{
		Vector3(-0.5f, 0.5f, 0.f),
		Vector3(0.5f, 0.5f, 0.f),
		Vector3(0.5f, -0.5f, 0.f),
		Vector3(-0.5f, -0.5f, 0.f),
	};
	const Matrix& BOX_WORLD_MATRIX = box->GetColliderWorldMatrix();
	Vector3 worldVertexPosArray[4] = {};

	for (int i = 0; i < 2; ++i)
	{
		worldVertexPosArray[i] = XMVector3TransformCoord(vertexPosArray[i], BOX_WORLD_MATRIX);
	}

	Vector3 right = (worldVertexPosArray[1] - worldVertexPosArray[0]);
	right.Normalize();

	float rotationRadinZ = acos(right.Dot(Vector3::Right));

	if (worldVertexPosArray[0].y > worldVertexPosArray[1].y)
	{
		rotationRadinZ = -rotationRadinZ;
	}

	const Matrix& BOX_ROTATION = Matrix::CreateRotationZ(-rotationRadinZ);

	Matrix boxZeroWorldMAtrix = BOX_WORLD_MATRIX;
	boxZeroWorldMAtrix._41 = 0.f; boxZeroWorldMAtrix._42 = 0.f; boxZeroWorldMAtrix._43 = 0.f;

	//rotating
	boxZeroWorldMAtrix = boxZeroWorldMAtrix * BOX_ROTATION;
	
	//Move to origin pos
	for (int i = 0; i < 4; ++i)
	{		
		worldVertexPosArray[i] = XMVector3TransformNormal(vertexPosArray[i], boxZeroWorldMAtrix);
	}

	//Move to origin pos
	Matrix MOVE_CIRCLE_MATRIX = {};
	MOVE_CIRCLE_MATRIX.Translation(-circle->GetColliderWorldMatrix().Translation());

	//Rotation radian
	const Matrix& CIRCLE_MATRIX = box->GetColliderWorldMatrix() * MOVE_CIRCLE_MATRIX;
	//Revolution
	const Vector3 CIRCLE_POS = (CIRCLE_MATRIX * BOX_ROTATION).Translation();

	//Circle pos Test
	Vector3 testPos = CIRCLE_POS;

	if (CIRCLE_POS.x < worldVertexPosArray[0].x)
	{
		testPos.x = worldVertexPosArray[0].x;
	}
	else if (CIRCLE_POS.x > worldVertexPosArray[1].x)
	{
		testPos.x = worldVertexPosArray[1].x;
	}

	if (CIRCLE_POS.y > worldVertexPosArray[0].y)
	{
		testPos.y = worldVertexPosArray[0].y;
	}
	else if (CIRCLE_POS.y < worldVertexPosArray[3].y)
	{
		testPos.y = worldVertexPosArray[3].y;
	}

	const Vector3& DIST = CIRCLE_POS - testPos;
	const float RADIUS = getCircleRadius(circle);


	return (DIST.x * DIST.x) + (DIST.y * DIST.y) <= RADIUS * RADIUS;
}

float CollisionManagement2D::getCircleRadius(const Collider2D* const collider) const
{
	constexpr const Vector3 vertexPos = Vector3(0.5f, 0.0f, 0.f);	

	Vector3 circleRadius = XMVector3TransformNormal(vertexPos, collider->GetColliderWorldMatrix());
	
	return circleRadius.Length();
}
