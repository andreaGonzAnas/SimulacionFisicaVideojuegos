#pragma once
#include "RigidBodySystem.h"
#include "ParticleGen.h"

class ProyectilRigidBodySystem : public RigidBodySystem
{
private:
	PxScene* _gScene;

public:
	ProyectilRigidBodySystem(PxPhysics* gPhysics, PxScene* gScene);
	~ProyectilRigidBodySystem();

	void update(double t) override;
	void createProyectil(physx::PxPhysics* gPhysics, physx::PxScene* gScene,
		physx::PxShape* shape, const physx::PxVec3& pos,
		const physx::PxVec3& vel, float density,
		double lifeTime, Vector4 color);
};

