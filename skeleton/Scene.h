#pragma once

#include "PxPhysicsAPI.h"
using namespace physx;

class Scene
{
protected:
    PxPhysics* gPhysics;

public:
    Scene(PxPhysics* physics) : gPhysics(physics) {}
    virtual ~Scene() {}

    virtual void init() = 0;
    virtual void update(double t) = 0;
    virtual void clear() = 0;

};

