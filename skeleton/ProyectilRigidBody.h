#pragma once
#include "core.hpp"

#include "DynamicObj.h"

class ProyectilRigidBody
{
private:
	double energiaS;
	double energiaR;

	DynamicObj* _obj;
public:
	ProyectilRigidBody(physx::PxPhysics* gPhysics, physx::PxScene* gScene,
		physx::PxShape* shape, const physx::PxVec3& pos,
		const physx::PxVec3& vel, float density,
		double lifeTime, Vector4 color);

	~ProyectilRigidBody();

	DynamicObj* getDynamic() { return _obj; }
	bool isDead() { return _obj->getTimeVida() <= 0; }
	void update(double t);
};

