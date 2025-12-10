#include "ExplosionRigidBodySystem.h"
#include "RigidBodyForceRegistry.h"
#include "UniformalRigidBodyGen.h"
#include "PxRigidDynamic.h"
#include "GravityForceGenerator.h"

#include "PxPhysicsAPI.h"


ExplosionRigidBodySystem::ExplosionRigidBodySystem(PxRigidDynamic* p, PxPhysics* gPhysics, PxScene* gScene): _gScene(gScene)
{
	//crear generador
    UniformalRigidBodyGen* confettiGen = new UniformalRigidBodyGen(5, 0.5, p, gPhysics);

    //setear desviaciones

    _generators.push_back(confettiGen);

    // generar partículas y moverlas a _rigidBodies
    if (!_generators.empty()) {
        std::list<DynamicObj*> newParticles = _generators.front()->generateP();
        if (!newParticles.empty())
            _rigidBodies.splice(_rigidBodies.end(), newParticles);
    }

    //añadir a escena
    for (auto r : _rigidBodies)
    {
        _gScene->addActor(*r->getRigidDynamic());
    }

    // Registro Fuerzas
    _registry = new RigidBodyForceRegistry();
    gravityEarth = new GravityForceGenerator();

    
    // Registrar fuerza de gravedad a todas las partículas
    for (auto p : _rigidBodies) {
        _registry->add(p->getRigidDynamic(), gravityEarth);
    }

}

ExplosionRigidBodySystem::~ExplosionRigidBodySystem()
{
}

void ExplosionRigidBodySystem::update(double t)
{
    //actualizar fuerzas
    _registry->updateForces(t);

    if (active)
    {
        // Generar nuevas partículas cada frame
        if (!_generators.empty()) {
            auto newParticles = _generators.front()->generateP();
            if (!newParticles.empty())
            {
                // Registrar cada nueva partícula en el registro de fuerzas
                for (auto p : _rigidBodies) {
                    _registry->add(p->getRigidDynamic(), gravityEarth);
                }

                for (auto r : newParticles)
                {
                    _gScene->addActor(*r->getRigidDynamic());
                }

                _rigidBodies.splice(_rigidBodies.end(), newParticles);
            }

        }
    }

    // Actualizar todas las existentes
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

void ExplosionRigidBodySystem::addForce()
{
}
