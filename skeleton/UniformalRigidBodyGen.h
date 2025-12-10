#pragma once
#include "RigidBodyGen.h"

class DynamicObj;

class UniformalRigidBodyGen : public RigidBodyGen
{
private:

public:
	UniformalRigidBodyGen(int nPart, double prob, PxRigidDynamic* p, PxPhysics* gPhysic);
	~UniformalRigidBodyGen();

	std::list<DynamicObj*> generateP() override;
};

