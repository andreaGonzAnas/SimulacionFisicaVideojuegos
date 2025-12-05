#pragma once

#include "RigidBodySystem.h"
#include "ParticleGen.h"

class ExplosionRigidBodySystem : public RigidBodySystem
{
private:

public:
	ExplosionRigidBodySystem(PxRigidDynamic* p, PxPhysics* gPhysics);
	~ExplosionRigidBodySystem();

	void update(double t) override;
	void addForce();
};

