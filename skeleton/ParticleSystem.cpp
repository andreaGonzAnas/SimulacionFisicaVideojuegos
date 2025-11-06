#include "ParticleSystem.h"
#include "UniformalGen.h"
#include "PxShape.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "WhirlwindForceGenerator.h"

#include <iostream>



ParticleSystem::ParticleSystem(): _particles()
{
	
}

ParticleSystem::~ParticleSystem()
{
    //eliminar todos los renderItems de las particulas creadas
    for (auto p : _particles)
    {
        if (p->getRenderItem()) {
            DeregisterRenderItem(p->getRenderItem());
            delete p->getRenderItem();
        }
        _registry->remove(p);
        delete p;
    }
}

void ParticleSystem::setActiveGravity(bool a) {
    if (gravityEarth != nullptr) gravityEarth->setActive(a);
}

void ParticleSystem::setActiveWind(bool a) {
    if (windForce != nullptr) windForce->setActive(a);
}

void ParticleSystem::setActiveWhirlWind(bool a) {
    if (whirlWindForce != nullptr) whirlWindForce->setActive(a);
}



