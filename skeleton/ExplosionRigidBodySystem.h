#pragma once

#include "RigidBodySystem.h"
#include "ParticleGen.h"

class ExplosionRigidBodySystem : public RigidBodySystem
{
private:
	PxScene* _gScene;

public:
	ExplosionRigidBodySystem(PxRigidDynamic* p, PxPhysics* gPhysics, PxScene* _gScene);
	~ExplosionRigidBodySystem();

	void update(double t) override;
	void addForce();
};

