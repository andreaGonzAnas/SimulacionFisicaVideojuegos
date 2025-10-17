#include "ParticleSystem.h"
#include "GaussianGen.h"

/*
ParticleSystem::ParticleSystem()
{
	
}*/

ParticleSystem::ParticleSystem(Particula* p)
{
	GaussianGen* gausGen = new GaussianGen(20, 0.5, p);
	_generators.push_back(gausGen);

	//generar particulas
	_particles = _generators.front()->generateP();
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::update(double t)
{
	for (auto p : _particles)
	{
		p->integrate(t);
	}
}
