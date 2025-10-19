#pragma once

#include <list>
#include "Particula.h"
#include "ParticleGen.h"

class PxShape;

class ParticleSystem
{
private:
	std::list<Particula*> _particles;
	std::list<ParticleGen*> _generators;
	//particula modelo
	Particula* _modelP;

public:
	//ParticleSystem();
	ParticleSystem(Particula* p);
	~ParticleSystem();

	void update(double t);
	void debugPrint();
};

