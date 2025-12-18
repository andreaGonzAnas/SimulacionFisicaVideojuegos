#pragma once
#include "RigidBodyGen.h"

class DynamicObj;

class UniformalRigidBodyGen : public RigidBodyGen
{
private:
	PxVec3 _originPos = PxVec3(0.0f, 0.0f, 0.0f); // Posición base

public:
	UniformalRigidBodyGen(int nPart, double prob, PxRigidDynamic* p, PxPhysics* gPhysic);
	~UniformalRigidBodyGen();

	std::list<DynamicObj*> generateP() override;
	void setOrigin(PxVec3 pos) { _originPos = pos; }
};

