#include "ParticleSystem.h"
#include "GaussianGen.h"
#include "PxShape.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"

#include <iostream>


/*
ParticleSystem::ParticleSystem()
{
	
}*/

ParticleSystem::ParticleSystem(Particula* p, PxPhysics* gPhysics): _particles()
{
	// crear generador
	GaussianGen* gausGen = new GaussianGen(5, 0.5, p, gPhysics);
	_generators.push_back(gausGen);

	// generar part�culas y moverlas a _particles
	if (!_generators.empty()) {
		auto newParticles = _generators.front()->generateP();
		if (!newParticles.empty())
			_particles.splice(_particles.end(), newParticles);
	}

    
    //crear registro fuerzas
    _registry = new ParticleForceRegistry();

    //a�adir fuerza gravitatoria
    // Crear dos generadores de gravedad diferentes
    gravityEarth = new GravityForceGenerator();

    // Registrar fuerzas
    for (auto p : _particles)
    {
        _registry->add(p, gravityEarth);
    }
    
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::update(double t)
{
    //actualizar fuerzas
    _registry->updateForces(t);
    
    // Generar nuevas part�culas cada frame
    if (!_generators.empty()) {
        auto newParticles = _generators.front()->generateP();
        if (!newParticles.empty())
        {
            // Registrar cada nueva part�cula en el registro de fuerzas
            for (auto p : newParticles) {
                _registry->add(p, gravityEarth); // _gravityGenerator es tu objeto GravityForceGenerator
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
                DeregisterRenderItem(p->getRenderItem()); // funci�n de tu motor
                delete p->getRenderItem();
            }

            delete p; // eliminar part�cula
            return true;
        }
        return false;
    });


}

