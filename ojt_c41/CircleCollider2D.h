#pragma once
#include "Collider2D.h"

REGISTER_COMPONENT_TYPE(CircleCollider2D);

class CircleCollider2D : public Collider2D
{
public:
	CircleCollider2D();
	virtual ~CircleCollider2D();
	//CircleCollider2D(const CircleCollider2D& other);
	CircleCollider2D& operator=(const CircleCollider2D&) = delete;

	CLONE(CircleCollider2D)
private:
	virtual void initialize() override final;
	virtual void update() override final;
	virtual void lateUpdate() override final;

};
