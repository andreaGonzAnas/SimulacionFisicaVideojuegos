#include "ProyectilRigidBody.h"

ProyectilRigidBody::ProyectilRigidBody(physx::PxPhysics* gPhysics, physx::PxScene* gScene, physx::PxShape* shape, const physx::PxVec3& pos, const physx::PxVec3& vel, float density, double lifeTime, Vector4 color)
{
    _obj = new DynamicObj(vel, physx::PxVec3(0, 0, 0), shape, pos, density, lifeTime, gPhysics, color);

    gScene->addActor(*_obj->getRigidDynamic());

}

ProyectilRigidBody::~ProyectilRigidBody()
{
    delete _obj;
}

void ProyectilRigidBody::update(double t)
{
    if (_obj) {
        _obj->update(t);
    }
}
