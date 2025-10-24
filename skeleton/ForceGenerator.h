#pragma once
#include "ParticleGen.h"

class ForceGenerator
{
private:

public:
	ForceGenerator() {};
	virtual void update(Particula* particle, double t) = 0;
};

