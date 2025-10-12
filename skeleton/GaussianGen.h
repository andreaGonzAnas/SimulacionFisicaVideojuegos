#pragma once

#include "ParticleGen.h"

class GaussianGen: public ParticleGen
{
private:
	std::normal_distribution<double> _d{ 0,1 };
	Particula* _modelP;

public:
	GaussianGen();
	//~GaussianGen() override;

	std::list<Particula*> generateP() override;

	void setModelP(Particula* p) { _modelP = p; }
};

