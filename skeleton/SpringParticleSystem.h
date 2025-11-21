#pragma once
#include "ParticleSystem.h"


class SpringParticleSystem : public ParticleSystem
{
public:
	SpringParticleSystem(PxMaterial* gMaterial);
	~SpringParticleSystem();

	void update(double t) override;
};

