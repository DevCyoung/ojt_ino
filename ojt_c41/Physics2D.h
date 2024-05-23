#pragma once
#include "EnumLayer.h"

class Collider2D;
class Scene;

struct RayCast2DHitInfo
{
	Vector2 hitPos;
	float distance;
	Collider2D* collider;
};

class Physics2D
{
public:
	Physics2D(Scene* const scene);
	virtual ~Physics2D();
	Physics2D(const Physics2D&) = delete;
	Physics2D& operator=(const Physics2D&) = delete;

	//Physics
	bool RayCastHit2D(const Vector2& origin,
		const Vector2& direction,
		const float distance,
		const eLayerType layerType,
		RayCast2DHitInfo* const outHitInfo);

	bool RayCastHit2D(const Vector2& origin,
		const Vector2& dest,
		const eLayerType layerType,
		RayCast2DHitInfo* const outHitInfo);	

	//bool checkCollision2DLineAndBox();
	//bool checkCollision2DLineAndCircle();
	//bool checkCollision2DLineAndLine();


private:
	Scene* mScene;
};

