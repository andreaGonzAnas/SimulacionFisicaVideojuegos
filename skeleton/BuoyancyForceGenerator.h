#pragma once
#include "ForceGenerator.h"
class BuoyancyForceGenerator : public ForceGenerator
{
protected:
	float _height;
	float _volume;
	float _liquid_density;
	float _gravity = 9.8;

	Particula* _liquid_particle;

public:
	BuoyancyForceGenerator(float h, float v, float d, Particula* liquidParticle);
	virtual ~BuoyancyForceGenerator() {}

	void update(Particula* particle, double t) override;

	void setVolume(float volume) { _volume = volume; }
	float getVolume() { return _volume; }
};

