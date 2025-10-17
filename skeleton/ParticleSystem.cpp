#include "ParticleSystem.h"
#include "GaussianGen.h"

/*
ParticleSystem::ParticleSystem()
{
	
}*/

ParticleSystem::ParticleSystem(Particula* p): _particles()
{
	// crear generador
	GaussianGen* gausGen = new GaussianGen(20, 0.5, p);
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
	_particles.front()->integrate(t);
	
	/*for (auto p : _particles)
	{
		p->integrate(t);
	}*/
}
