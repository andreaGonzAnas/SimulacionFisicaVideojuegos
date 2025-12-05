#pragma once
#include <list>
#include "PxPhysicsAPI.h"

class ParticleForceRegistry;

class RigidBodySystem
{
protected:
	std::list<PxRigidDynamic*> _particles;
	PxRigidDynamic* _modelP; //rigid body modelo

	//registro de fuerzas
	ParticleForceRegistry* _registry;

	bool active = true;

public:
	RigidBodySystem();
	~RigidBodySystem();

	virtual void update(double t) {};

	void setActive(bool a) { active = a; };
	bool isActive() { return active; };
};

