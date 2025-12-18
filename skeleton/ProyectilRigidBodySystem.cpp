#include "ProyectilRigidBodySystem.h"
#include "RigidBodyForceRegistry.h"
#include "GravityForceGenerator.h"
#include "ProyectilRigidBody.h"

ProyectilRigidBodySystem::ProyectilRigidBodySystem(PxPhysics* gPhysics, PxScene* gScene): _gScene(gScene)
{
    //En este caso no se generan desde un generador, sino que se llama al createProyectil

    //crear registro fuerzas
    _registry = new RigidBodyForceRegistry();

    //añadir fuerza gravitatoria
    // Crear dos generadores de gravedad diferentes
    gravityEarth = new GravityForceGenerator();

    // Registrar fuerzas
    for (auto p : _rigidBodies)
    {
        _registry->add(p->getRigidDynamic(), gravityEarth);
    }

    //inicializar a null los que no queremos
    windForce = nullptr;
}

ProyectilRigidBodySystem::~ProyectilRigidBodySystem()
{
}

void ProyectilRigidBodySystem::update(double t)
{
    //actualizar fuerzas
    _registry->updateForces(t);


    for (auto it = _rigidBodies.begin(); it != _rigidBodies.end(); )
    {
        DynamicObj* obj = *it;
        obj->update(t);

        if (obj->getTimeVida() <= 0.0)
        {
            _registry->remove(obj->getRigidDynamic());
            _gScene->removeActor(*obj->getRigidDynamic());
            delete obj;  // llama al destructor que libera el rigid body y renderItem
            it = _rigidBodies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void ProyectilRigidBodySystem::createProyectil(physx::PxPhysics* gPhysics, physx::PxScene* gScene, physx::PxShape* shape, const physx::PxVec3& pos, const physx::PxVec3& vel, float density, double lifeTime, Vector4 color)
{
    ProyectilRigidBody* p = new ProyectilRigidBody(gPhysics, gScene, shape, pos, vel, density, lifeTime, color);
    _registry->add(p->getDynamic()->getRigidDynamic(), gravityEarth);
    _rigidBodies.push_back(p->getDynamic());
}
