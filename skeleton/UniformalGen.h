#pragma once

#include "ParticleGen.h"

class UniformalGen: public ParticleGen
{
private:
	std::normal_distribution<double> _d{ 0,1 };

public:
	UniformalGen();
	~UniformalGen();

	void generateP() override;
};

