#pragma once
#include "RenderComponent.h"

REGISTER_COMPONENT_TYPE(MeshRenderer);

class MeshRenderer : public RenderComponent
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();
	//MeshRenderer(const MeshRenderer&);
	MeshRenderer& operator=(const MeshRenderer&) = delete;

	virtual void Save(FILE* const file);
	virtual void Load(FILE* const file);

	CLONE(MeshRenderer);
	virtual void render(const Matrix& viewMatrix, const Matrix& projectionMatrix) override final;
};

