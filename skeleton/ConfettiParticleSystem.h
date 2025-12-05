#pragma once
#include "ParticleSystem.h"

class ConfettiParticleSystem : public ParticleSystem
{
private:
    double spawnTimer;
    double spawnInterval;
public:
    ConfettiParticleSystem(Particula* p, PxPhysics* gPhysics);
    ~ConfettiParticleSystem();

    void update(double t) override;
};

