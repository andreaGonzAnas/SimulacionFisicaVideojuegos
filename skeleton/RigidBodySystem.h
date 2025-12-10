#pragma once
#include <list>
#include "PxPhysicsAPI.h"

class ParticleForceRegistry;
class RigidBodyGen;
class GravityForceGenerator;

class RigidBodySystem
{
protected:
	std::list<physx::PxRigidDynamic*> _rigidBodies;
	physx::PxRigidDynamic* _modelP; //rigid body modelo

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

	std::list<physx::PxRigidDynamic*> getRigidBodies() { return _rigidBodies;}
};

