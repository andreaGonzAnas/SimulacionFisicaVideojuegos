#include "ExplosionRigidBodySystem.h"
#include "ParticleForceRegistry.h"
#include "UniformalRigidBodyGen.h"
#include "PxRigidDynamic.h"
#include "GravityForceGenerator.h"

#include "PxPhysicsAPI.h"


ExplosionRigidBodySystem::ExplosionRigidBodySystem(PxRigidDynamic* p, PxPhysics* gPhysics)
{
	//crear generador
    UniformalRigidBodyGen* confettiGen = new UniformalRigidBodyGen(5, 0.5, p, gPhysics);

    //setear desviaciones

    _generators.push_back(confettiGen);

    // generar partículas y moverlas a _rigidBodies
    if (!_generators.empty()) {
        std::list<PxRigidDynamic*> newParticles = _generators.front()->generateP();
        if (!newParticles.empty())
            _rigidBodies.splice(_rigidBodies.end(), newParticles);
    }

    // Registro Fuerzas
    _registry = new ParticleForceRegistry();
    gravityEarth = new GravityForceGenerator();

    // Registrar fuerza de gravedad a todas las partículas
    for (auto p : _rigidBodies) {
        _registry->add(p, gravityEarth);
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
                for (auto p : newParticles) {
                    _registry->add(p, gravityEarth);
                }
                _rigidBodies.splice(_rigidBodies.end(), newParticles);
            }

        }
    }

    // Actualizar todas las existentes
    /*
    for (auto p : _particles) {
        p->integrate(t);
    }*/

    _rigidBodies.remove_if([this](Particula* p) {
        if (p->getTimeVida() <= 0.0) {
            if (p->getRenderItem()) {
                DeregisterRenderItem(p->getRenderItem());
                delete p->getRenderItem();
            }
            _registry->remove(p);
            delete p;
            return true;
        }
        return false;
        });
}

void ExplosionRigidBodySystem::addForce()
{
}
