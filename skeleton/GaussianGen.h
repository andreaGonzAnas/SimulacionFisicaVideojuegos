#pragma once

#include "ParticleGen.h"

class GaussianGen: public ParticleGen
{
private:
	std::normal_distribution<double> _d{ 0,1 };

public:
	GaussianGen();
	~GaussianGen();

	void generateP() override;

};

