#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

class DynamicObj
{
private:
	double tVida;
	physx::PxPhysics* _gPhysic;
	physx::PxRigidDynamic* _rigidDynamic;
	RenderItem* _renderItem;

public:
	DynamicObj(const physx::PxVec3& linVel, const physx::PxVec3& angVel, physx::PxShape*, const physx::PxVec3& position, 
		physx::PxReal density, double time, physx::PxPhysics* gPhysic, Vector4 color);
	~DynamicObj();
	void update(double t);
	physx::PxRigidDynamic* getRigidDynamic() { return _rigidDynamic; }
	double getTimeVida() { return tVida;}
	RenderItem* getRenderItem() { return _renderItem; }
	void setRenderItem(RenderItem* r) {
		_renderItem = r;
		RegisterRenderItem(_renderItem);}
};

