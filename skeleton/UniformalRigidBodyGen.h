#pragma once
#include "RigidBodyGen.h"

class UniformalRigidBodyGen : public RigidBodyGen
{
private:

public:
	UniformalRigidBodyGen(int nPart, double prob, PxRigidDynamic* p, PxPhysics* gPhysic);
	~UniformalRigidBodyGen();

	std::list<PxRigidDynamic*> generateP() override;
};

