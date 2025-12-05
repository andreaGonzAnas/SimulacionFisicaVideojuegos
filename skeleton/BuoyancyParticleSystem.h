#pragma once
#include "ParticleSystem.h"

class BuoyancyForceGenerator;

class BuoyancyParticleSystem : public ParticleSystem
{
private:
	Particula* pFlot;
	BuoyancyForceGenerator* f;
public:
	BuoyancyParticleSystem(PxMaterial* gMaterial);
	~BuoyancyParticleSystem();

	void update(double t) override;

	void setMasa(double cant);
	void setVolume(double cant);
};

