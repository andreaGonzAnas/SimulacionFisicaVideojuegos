#pragma once

#include "ParticleGen.h"

class UniformalGen: public ParticleGen
{
private:

public:
	UniformalGen(int nPart, double prob, Particula* p, PxPhysics* gPhysic);
	~UniformalGen();

	std::list<Particula*> generateP() override;
};

