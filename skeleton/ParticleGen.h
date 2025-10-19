#pragma once

#include <random>
#include "core.hpp"

#include "Particula.h"
#include <list>

#include "PxPhysicsAPI.h"
using namespace physx;


class ParticleGen
{
protected:
	int nParticulas;
	std::mt19937 _mt;
	std::uniform_real_distribution<double> _u{ 0.0,1.0 };
	Vector3 desP;
	Vector3 desVel;
	double desDur;
	double probGeneration;
	Vector4 desColor;

	PxPhysics* gPhysics;

public:
	ParticleGen();
	//virtual ~ParticleGen();
	virtual std::list<Particula*> generateP() = 0;

	//getters y setters
	Vector3 getInitialPos() { return desP; }
	void setInitialPos(Vector3 pos) { desP = pos; }
	
	Vector3 getVel() { return desP; }
	void setVel(Vector3 vel) { desVel = vel; }

	double getDuration() { return desDur; }
	void setDuration(double duration) { desDur = duration; }

	double getProbGen() { return probGeneration; }
	void setProbGen(double prob) { probGeneration = prob; }

	int getParticulas() { return nParticulas; }
	void setParticulas(int n) { nParticulas = n; }
};

