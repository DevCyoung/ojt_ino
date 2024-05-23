#pragma once
#include "RenderComponent.h"

REGISTER_COMPONENT_TYPE(ParticleSystem);

class Mesh;
class Material;
class Texture;


class ParticleSystem : public RenderComponent
{
public:
	ParticleSystem();
	virtual ~ParticleSystem();
	ParticleSystem(const ParticleSystem& other);
	ParticleSystem& operator=(const ParticleSystem&) = delete;

	CLONE(ParticleSystem)

private:
	virtual void initialize() override final;
	virtual void update() override final;
	virtual void lateUpdate() override final;
	virtual void render(const Matrix& viewMatrix, const Matrix& projectionMatrix) override final;
};

