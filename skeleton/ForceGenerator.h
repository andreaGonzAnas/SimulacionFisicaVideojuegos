#pragma once
#include "ParticleGen.h"
#include <iostream>

class ForceGenerator
{
protected:
	bool active = true;
public:
	ForceGenerator() {};
	virtual void update(Particula* particle, double t) = 0;
	virtual void updateRigidBody(physx::PxRigidDynamic* b, double t) {} // rigid bodies

	void setActive(bool a) { active = a; }
	bool isActive() const { return active; }
};

