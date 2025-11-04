#include "ParticleSystem.h"
#include "UniformalGen.h"
#include "PxShape.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"

#include <iostream>


/*
ParticleSystem::ParticleSystem()
{
	
}*/

ParticleSystem::ParticleSystem(Particula* p, PxPhysics* gPhysics): _particles()
{
	// crear generador
    UniformalGen* gausGen = new UniformalGen(10, 0.5, p, gPhysics);
	_generators.push_back(gausGen);

	// generar partículas y moverlas a _particles
	if (!_generators.empty()) {
		auto newParticles = _generators.front()->generateP();
		if (!newParticles.empty())
			_particles.splice(_particles.end(), newParticles);
	}

    //crear registro fuerzas
    _registry = new ParticleForceRegistry();

    //windForce = new WindForceGenerator(Vector3(6.0, 6.0, 0.0), 0.04, 0);

    //// Asignar a partículas
    //for (auto p : _particles) {
    //    _registry->add(p, windForce);
    //}

    //Fuerza explosion
    explosionForce = new WhirlwindForceGenerator(Vector3(35, 35, 35), 2, 1, 2);

    // Asignar a partículas
    for (auto p : _particles) {
        _registry->add(p, explosionForce);
    }
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::update(double t)
{
    //actualizar fuerzas
    _registry->updateForces(t);
    
    // Generar nuevas partículas cada frame
    if (!_generators.empty()) {
        auto newParticles = _generators.front()->generateP();
        if (!newParticles.empty())
        {
            // Registrar cada nueva partícula en el registro de fuerzas
            for (auto p : newParticles) {
                //_registry->add(p, gravityEarth);
                //_registry->add(p, windForce); 
                _registry->add(p, explosionForce);
            }
            _particles.splice(_particles.end(), newParticles);
        }
            
    }

    // Actualizar todas las existentes
    for (auto p : _particles) {
        p->integrate(t);
    }

    _particles.remove_if([this](Particula* p) {
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

