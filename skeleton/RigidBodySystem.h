#pragma once
#include <list>
#include "PxPhysicsAPI.h"
#include "DynamicObj.h"

class RigidBodyForceRegistry;
class RigidBodyGen;
class GravityForceGenerator;

class RigidBodySystem
{
protected:
	std::list<DynamicObj*> _rigidBodies;
	physx::PxRigidDynamic* _modelP; //rigid body modelo

	//registro de fuerzas
	RigidBodyForceRegistry* _registry;
	std::list<RigidBodyGen*> _generators;

	GravityForceGenerator* gravityEarth;

	bool active = true;

public:
	RigidBodySystem();
	~RigidBodySystem();

	virtual void update(double t) {};

	void setActive(bool a) { active = a; };
	bool isActive() { return active; };

	std::list<DynamicObj*> getRigidBodies() { return _rigidBodies;}
};

