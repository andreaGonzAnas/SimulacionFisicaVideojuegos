#pragma once
#include "core.hpp"

class Particula;

class Proyectil
{
private:
	double energiaS;
	double energiaR;

	Particula* p;
public:
	Proyectil(Vector4 color, double masaR, double velR, double velS, physx::PxShape* esferaShape);

	Particula* getParticle() { return p; }

};

