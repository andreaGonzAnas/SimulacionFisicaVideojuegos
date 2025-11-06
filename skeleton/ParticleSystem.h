#pragma once

#include <list>
#include "Particula.h"
#include "ParticleGen.h"

class PxShape;
class ParticleForceRegistry;
class GravityForceGenerator;
class WindForceGenerator;
class WhirlwindForceGenerator;

class ParticleSystem
{
protected:
	std::list<Particula*> _particles;
	std::list<ParticleGen*> _generators;
	//particula modelo
	Particula* _modelP;

	//registro de fuerzas
	ParticleForceRegistry* _registry;

	GravityForceGenerator* gravityEarth;
	WindForceGenerator* windForce;
	WhirlwindForceGenerator* whirlWindForce;

	bool active = true;

public:
	ParticleSystem();
	~ParticleSystem();

	virtual void update(double t) {};

	std::list<Particula*> getParticleList() {return _particles;}

	void ParticleSystem::setActiveGravity(bool);
	void ParticleSystem::setActiveWind(bool);
	void ParticleSystem::setActiveWhirlWind(bool);

	void ParticleSystem::setActive(bool a) { active = a; };
	bool ParticleSystem::isActive() { return active; };
};

