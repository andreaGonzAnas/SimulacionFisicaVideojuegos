#pragma once

#include <list>
#include "Particula.h"
#include "ParticleGen.h"

class ParticleSystem
{
private:
	std::list<Particula*> _particles;
	std::list<ParticleGen*> _generators;
	//particula modelo

public:
	ParticleSystem();
	~ParticleSystem();

	void update(double t);
};

