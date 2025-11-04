#pragma once

#include "ParticleSystem.h"

#pragma once
class FireworksParticleSystem: public ParticleSystem
{
private:
	double contExplosion;
	bool active;

	int generation = 1;       // 0 = partícula inicial
	bool exploded = false;    // evita que explote varias veces

public:
	FireworksParticleSystem(Particula* p, PxPhysics*);
	~FireworksParticleSystem();

	void update(double t) override;
};

