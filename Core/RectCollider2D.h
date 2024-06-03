#pragma once
#include "Collider2D.h"

REGISTER_COMPONENT_TYPE(RectCollider2D);

class RectCollider2D : public Collider2D
{
public:
	RectCollider2D();
	virtual ~RectCollider2D();
	//RectCollider2D(const RectCollider2D& other);
	RectCollider2D& operator=(const RectCollider2D&) = delete;

	CLONE(RectCollider2D)

private:
	virtual void initialize() override final;
	virtual void update() override final;
	virtual void lateUpdate() override final;	
};
