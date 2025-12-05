#pragma once
#include "ParticleSystem.h"

#include <vector>

class SpringForceGenerator;

class SpringParticleSystem : public ParticleSystem
{
private:
	std::vector<SpringForceGenerator*> _vFuerzas;
public:
	SpringParticleSystem(PxMaterial* gMaterial);
	~SpringParticleSystem();

	void update(double t) override;
	void setK(double cant);
};

