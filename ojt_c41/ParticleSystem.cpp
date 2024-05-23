#include "pch.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
	: RenderComponent(eComponentType::ParticleSystem)
{
}

ParticleSystem::~ParticleSystem()
{
}

ParticleSystem::ParticleSystem(const ParticleSystem& other)
	: RenderComponent(other)
{
}

void ParticleSystem::initialize()
{
}

void ParticleSystem::update()
{
	RenderComponent::update();
}

void ParticleSystem::lateUpdate()
{
}

void ParticleSystem::render(const Matrix& viewMatrix, const Matrix& projectionMatrix)
{
	(void)viewMatrix;
	(void)projectionMatrix;

	//(void)camera;
}
