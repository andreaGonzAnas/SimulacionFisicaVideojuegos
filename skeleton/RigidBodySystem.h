#pragma once
#include <list>
#include "PxPhysicsAPI.h"

class ParticleForceRegistry;
class RigidBodyGen;
class GravityForceGenerator;

class RigidBodySystem
{
protected:
	std::list<PxRigidDynamic*> _rigidBodies;
	PxRigidDynamic* _modelP; //rigid body modelo

	//registro de fuerzas
	ParticleForceRegistry* _registry;
	std::list<RigidBodyGen*> _generators;

	GravityForceGenerator* gravityEarth;

	bool active = true;

public:
	RigidBodySystem();
	~RigidBodySystem();

	virtual void update(double t) {};

	void setActive(bool a) { active = a; };
	bool isActive() { return active; };
};

