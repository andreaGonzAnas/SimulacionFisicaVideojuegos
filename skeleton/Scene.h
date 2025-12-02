#pragma once

#include "PxPhysicsAPI.h"
using namespace physx;

class Scene
{
protected:
    PxPhysics* gPhysics;
    PxScene* _gScene;

public:
    Scene(PxPhysics* physics) : gPhysics(physics) {}
    virtual ~Scene() {}

    virtual void init() = 0;
    virtual void update(double t) = 0;
    virtual void clear() = 0;

    // input
    virtual bool handleKey(unsigned char key, const PxTransform& camera) = 0;

    PxScene* get_gScene()
    {
        return _gScene;
    }

    void set_gScene(PxScene* scene)
    {
        _gScene = scene;
    }

};

