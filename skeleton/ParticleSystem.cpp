#include "ParticleSystem.h"
#include "GaussianGen.h"
#include "PxShape.h"
#include <iostream>


/*
ParticleSystem::ParticleSystem()
{
	
}*/

ParticleSystem::ParticleSystem(Particula* p, PxPhysics* gPhysics): _particles()
{
	// crear generador
	GaussianGen* gausGen = new GaussianGen(10, 0.5, p, gPhysics);
	_generators.push_back(gausGen);

	// generar partículas y moverlas a _particles
	if (!_generators.empty()) {
		auto newParticles = _generators.front()->generateP();
		if (!newParticles.empty())
			_particles.splice(_particles.end(), newParticles);
	}
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::update(double t)
{
    
    // Generar nuevas partículas cada frame
    if (!_generators.empty()) {
        auto newParticles = _generators.front()->generateP();
        if (!newParticles.empty())
            _particles.splice(_particles.end(), newParticles);
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

