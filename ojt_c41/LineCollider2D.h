#pragma once
#include "Collider2D.h"

REGISTER_COMPONENT_TYPE(LineCollider2D);

class LineCollider2D : public Collider2D
{
public:
	LineCollider2D();
	virtual ~LineCollider2D();
	//LineCollider2D(const LineCollider2D&);
	LineCollider2D& operator=(const LineCollider2D&) = delete;

	CLONE(LineCollider2D)
private:
	virtual void initialize() override final;
	virtual void update() override final;
	virtual void lateUpdate() override final;

};
