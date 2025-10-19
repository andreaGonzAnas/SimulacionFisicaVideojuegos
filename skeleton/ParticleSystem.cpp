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
	GaussianGen* gausGen = new GaussianGen(20, 0.5, p, gPhysics);
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
    /*
    // Generar nuevas partículas cada frame
    if (!_generators.empty()) {
        auto newParticles = _generators.front()->generateP();
        if (!newParticles.empty())
            _particles.splice(_particles.end(), newParticles);
    }*/

    // Actualizar todas las existentes
    for (auto p : _particles) {
        p->integrate(t);
    }

    //_particles.front()->integrate(t);

    // LIMPIAR muertas (ver apartado 5)
    debugPrint(); // después

}

void ParticleSystem::debugPrint()
{
    std::cout << "ParticleSystem: count = " << _particles.size() << std::endl;
    int i = 0;
    for (auto& p : _particles) {
        if (i++ >= 5) break;
        auto pos = p->getTransform()->p; // o getPrePos() según tu API de Particula
        std::cout << "  p[" << (&p) << "] pos=(" << pos.x << "," << pos.y << "," << pos.z
            << ") life=" << p->getTimeVida() << std::endl;
    }
}


