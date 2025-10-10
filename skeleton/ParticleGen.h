#pragma once

#include <random>
#include "core.hpp"

#include "Particula.h"
#include <list>

class ParticleGen
{
private:
	Vector3 initialPos;
	Vector3 initialVel;
	double initialDuration;
	double probGeneration;

protected:
	int nParticulas;
	std::mt19937 _mt;
	std::uniform_real_distribution<double> _u{ 0,1 };

public:
	ParticleGen();
	virtual ~ParticleGen();
	virtual std::list<Particula*> generateP() = 0;

	//getters y setters
	Vector3 getInitialPos() { return initialPos; }
	void setInitialPos(Vector3 pos) { initialPos = pos; }
	
	Vector3 getVel() { return initialPos; }
	void setVel(Vector3 vel) { initialVel = vel; }

	double getDuration() { return initialDuration; }
	void setDuration(double duration) { initialDuration = duration; }

	double getProbGen() { return probGeneration; }
	void setProbGen(double prob) { probGeneration = prob; }
};

