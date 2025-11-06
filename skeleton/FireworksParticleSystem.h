#pragma once

#include "ParticleSystem.h"

#pragma once
class FireworksParticleSystem: public ParticleSystem
{
private:
	double contExplosion;

	int generation = 1;       // 0 = partícula inicial
	bool exploded = false;    // evita que explote varias veces
	bool end = false;

public:
	FireworksParticleSystem(Particula* p, PxPhysics*);
	~FireworksParticleSystem();

	void update(double t) override;

	bool getEnd() { return end; }
};

