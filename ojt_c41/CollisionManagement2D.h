#pragma once
#include "EnumLayer.h"

class Scene;
class Collider2D;
class GameObject;

class CollisionManagement2D
{
	friend class Scene;
public:
	CollisionManagement2D();
	virtual ~CollisionManagement2D();
	CollisionManagement2D(const CollisionManagement2D&) = delete;
	CollisionManagement2D& operator=(const CollisionManagement2D&) = delete;

	void TurnOnAllCollisionLayer();
	void TurnOffAllCollisionLayer();
	void TurnOnCollisionLayer(const eLayerType left, const eLayerType right);
	void TurnOffCollisionLayer(const eLayerType left, const eLayerType right);

private:
	void phisicsUpdate(const Scene* const scene);
	void collide2DGameObject(const GameObject* const left, const GameObject* const right);

	bool checkCollision2DBoxAndBox(const Collider2D* const left, const Collider2D* const right);
	bool checkCollision2DBoxAndCircle(const Collider2D* const box, const Collider2D* const circle);
	bool checkCollision2DBoxAndLine(const Collider2D* const box, const Collider2D* const line);

	bool checkCollision2DCircleAndCircle(const Collider2D* const left, const Collider2D* const right);
	bool checkCollision2DLineAndLine(const Collider2D* const left, const Collider2D* const right);

	float getCircleRadius(const Collider2D* const collider) const;

private:
	union CollisionID
	{
		struct tagLayerID
		{
			UINT LeftID;
			UINT RightID;
		} entityID;		

		UINT_PTR id;
	};

	UINT	mCollisionMatrix[static_cast<UINT>(eLayerType::End)];
	std::unordered_map<UINT_PTR, bool>	mCollisionCheckMap;
};

