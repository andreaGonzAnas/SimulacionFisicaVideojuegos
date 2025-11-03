#include "ParticleSystem.h"
#include "GaussianGen.h"
#include "PxShape.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"

#include <iostream>


/*
ParticleSystem::ParticleSystem()
{
	
}*/

ParticleSystem::ParticleSystem(Particula* p, PxPhysics* gPhysics): _particles()
{
	// crear generador
	GaussianGen* gausGen = new GaussianGen(2, 0.5, p, gPhysics);
	_generators.push_back(gausGen);

	// generar partículas y moverlas a _particles
	if (!_generators.empty()) {
		auto newParticles = _generators.front()->generateP();
		if (!newParticles.empty())
			_particles.splice(_particles.end(), newParticles);
	}

    //crear registro fuerzas
    _registry = new ParticleForceRegistry();

    //añadir fuerza gravitatoria
    // Crear dos generadores de gravedad diferentes
    gravityEarth = new GravityForceGenerator();

    // Registrar fuerzas
    for (auto p : _particles)
    {
        _registry->add(p, gravityEarth);
    }

    windForce = new WindForceGenerator(Vector3(50.0, 50.0, 0.0), 5.0, 1.0);

    // Asignar a partículas
    for (auto p : _particles) {
        _registry->add(p, windForce);
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
                _registry->add(p, gravityEarth);
                _registry->add(p, windForce); 
            }
            _particles.splice(_particles.end(), newParticles);
        }
            
    }

    // Actualizar todas las existentes
    for (auto p : _particles) {
        p->integrate(t);
    }

    _particles.remove_if([](Particula* p) {
        if (p->getTimeVida() <= 0.0) {
            // eliminar render item si existe
            if (p->getRenderItem()) {
                DeregisterRenderItem(p->getRenderItem()); // función de tu motor
                delete p->getRenderItem();
            }

            delete p; // eliminar partícula
            return true;
        }
        return false;
    });


}

