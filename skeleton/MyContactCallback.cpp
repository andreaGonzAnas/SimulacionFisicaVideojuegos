#include "MyContactCallback.h"
#include "SceneTrapecios.h"

using namespace physx;


MyContactCallback::MyContactCallback(SceneTrapecios* _scene): scene(_scene)
{
    scene->get_gScene()->setSimulationEventCallback(this);
}

MyContactCallback::~MyContactCallback()
{
    scene->get_gScene()->setSimulationEventCallback(nullptr);
}

void MyContactCallback::onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
{

}

void MyContactCallback::onWake(PxActor** actors, PxU32 count)
{
}

void MyContactCallback::onSleep(PxActor** actors, PxU32 count)
{
}

void MyContactCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
    //logica colision
    for (PxU32 i = 0; i < nbPairs; i++)
    {
        if (pairs[i].events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            // pasar los dos actores de colision
            scene->handleContact(pairHeader.actors[0], pairHeader.actors[1]);
        }
    }
}

void MyContactCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
}

void MyContactCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}
