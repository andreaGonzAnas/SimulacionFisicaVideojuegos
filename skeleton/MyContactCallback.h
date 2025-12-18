#pragma once
#include "../common/PhysX-3.4/PhysX_3.4/Include/PxSimulationEventCallback.h"
#include "PxPhysicsAPI.h"



class SceneTrapecios;

class MyContactCallback : public physx::PxSimulationEventCallback
{
public:
    SceneTrapecios* scene;

	void onContact(
		const physx::PxContactPairHeader& pairHeader,
		const physx::PxContactPair* pairs,
		physx::PxU32 nbPairs
	) override;

	MyContactCallback(SceneTrapecios* scene);
	~MyContactCallback();

	// Heredado vía PxSimulationEventCallback
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
	void onWake(physx::PxActor** actors, physx::PxU32 count) override;
	void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
	void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;

};

