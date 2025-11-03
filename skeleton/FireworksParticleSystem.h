#pragma once

#include <list>
#include "Particula.h"
#include "ParticleGen.h"

class PxShape;
class ParticleForceRegistry;
class GravityForceGenerator;

#pragma once
class FireworksParticleSystem
{
private:
	std::list<Particula*> _particles;
	std::list<ParticleGen*> _generators;
	//particula modelo
	Particula* _modelP;

	//registro de fuerzas
	ParticleForceRegistry* _registry;

	GravityForceGenerator* gravityEarth;

	double contExplosion;
	bool active;

	int generation = 1;       // 0 = partícula inicial
	bool exploded = false;    // evita que explote varias veces

public:
	FireworksParticleSystem(Particula* p, PxPhysics*);
	~FireworksParticleSystem();

	void update(double t);

	std::list<Particula*> getParticleList() { return _particles; }
};

