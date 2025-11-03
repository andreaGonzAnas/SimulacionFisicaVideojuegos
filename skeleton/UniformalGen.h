#pragma once

#include "ParticleGen.h"

class UniformalGen: public ParticleGen
{
private:
	std::normal_distribution<double> _d{ 0,1 };

public:
	UniformalGen(int nPart, double prob, Particula* p, PxPhysics* gPhysic);
	~UniformalGen();

	std::list<Particula*> generateP() override;
};

