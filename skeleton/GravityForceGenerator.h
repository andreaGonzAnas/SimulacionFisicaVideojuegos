#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
private:
	//atributos de la fuerza gravitatoria
	PxVec3 gravity = PxVec3(0.0,-9.8, 0.0);

public:
	//Constructora
	GravityForceGenerator();

	//Update
	void update(Particula* particle, double t) override;
	void updateRigidBody(physx::PxRigidDynamic* b, double t) override;

};

