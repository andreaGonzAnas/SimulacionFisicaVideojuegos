#pragma once

#include "ParticleGen.h"

class PxShape;

class GaussianGen: public ParticleGen
{
private:
	std::normal_distribution<double> _d{ 0.0,1.0 };
	physx::PxShape* _Pshape;

public:
	GaussianGen();
	GaussianGen(int nPart, double p, Particula*, PxPhysics* gPhysics);
	//~GaussianGen() override;

	std::list<Particula*> generateP() override;
};

