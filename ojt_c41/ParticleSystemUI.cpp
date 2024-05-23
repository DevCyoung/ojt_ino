# include "pch.h"
#include "ParticleSystem.h"
class RenderComponent;
void RenderComponentUI(RenderComponent* component);


void ParticleSystemUI(ParticleSystem* component)
{
	RenderComponentUI(component);

}
