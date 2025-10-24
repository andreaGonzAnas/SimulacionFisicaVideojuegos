#pragma once

#include <list>
#include "Particula.h"
#include "ParticleGen.h"

class PxShape;
class ParticleForceRegistry;
class GravityForceGenerator;

class ParticleSystem
{
private:
	std::list<Particula*> _particles;
	std::list<ParticleGen*> _generators;
	//particula modelo
	Particula* _modelP;

	//registro de fuerzas
	ParticleForceRegistry* _registry;

	GravityForceGenerator* gravityEarth;

public:
	//ParticleSystem();
	ParticleSystem(Particula* p, PxPhysics*);
	~ParticleSystem();

	void update(double t);
	void debugPrint();

	std::list<Particula*> getParticleList() {return _particles;}
};

