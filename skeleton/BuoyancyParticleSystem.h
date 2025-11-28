#pragma once
#include "ParticleSystem.h"
class BuoyancyParticleSystem : public ParticleSystem
{
public:
	BuoyancyParticleSystem(PxMaterial* gMaterial);
	~BuoyancyParticleSystem();

	void update(double t) override;
};

