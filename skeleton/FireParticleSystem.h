#pragma once
#include "ParticleSystem.h"

class FireParticleSystem : public ParticleSystem
{
private:

public:
	FireParticleSystem(Particula* p, PxPhysics*);
	~FireParticleSystem();

	void update(double t) override;
};

