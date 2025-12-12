#include "DynamicObj.h"

using namespace physx;


DynamicObj::DynamicObj(const physx::PxVec3& linVel, const physx::PxVec3& angVel, physx::PxShape* shape, const physx::PxVec3& position, physx::PxReal density, double time, PxPhysics* gPhysic, Vector4 color): tVida(time), _gPhysic(gPhysic)
{
	//physx::PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));

	//crear rigid body dinamico
	PxRigidDynamic* new_solid;
	new_solid = _gPhysic->createRigidDynamic(PxTransform(position));
	new_solid->setLinearVelocity(linVel);
	new_solid->setAngularVelocity(angVel);
	new_solid->attachShape(*shape);

	PxRigidBodyExt::updateMassAndInertia(*new_solid, density);

	// Pintar actor dinamico
	_renderItem = new RenderItem(shape, new_solid, color);

	_rigidDynamic = new_solid;
}

DynamicObj::~DynamicObj()
{
	
}

void DynamicObj::update(double t)
{
	//actualizar tiempo
	tVida -= t;
	if (tVida < 0.0) tVida = 0.0;
}
