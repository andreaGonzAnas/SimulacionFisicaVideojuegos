#pragma once
#include "ParticleSystem.h"

#include <vector>

class SpringForceGenerator;

class SpringParticleSystem : public ParticleSystem
{
private:
	std::vector<SpringForceGenerator*> _vFuerzas;
	Particula* p3;

	double pushTime = 0.0; // tiempo para el empuje
	Vector3 pushForce = { 0,0,0 }; // vector fuerza
public:
	SpringParticleSystem(PxMaterial* gMaterial);
	~SpringParticleSystem();

	void update(double t) override;
	void setK(double cant);
	void addForce();
};

