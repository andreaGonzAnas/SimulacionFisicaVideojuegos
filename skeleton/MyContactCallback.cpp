#include "MyContactCallback.h"
#include "SceneTrapecios.h"
#include "InitialMenuScene.h"

using namespace physx;


MyContactCallback::MyContactCallback(SceneTrapecios* _scene, InitialMenuScene* _initialMenu): scene(_scene), initialMenu(_initialMenu)
{
    if (scene) scene->get_gScene()->setSimulationEventCallback(this);
    if (initialMenu) initialMenu->get_gScene()->setSimulationEventCallback(this);
}

MyContactCallback::~MyContactCallback()
{
    if (scene) scene->get_gScene()->setSimulationEventCallback(nullptr);
    if (initialMenu) initialMenu->get_gScene()->setSimulationEventCallback(nullptr);
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
            if(scene) scene->handleContact(pairHeader.actors[0], pairHeader.actors[1]);

            if(initialMenu) initialMenu->handleContact(pairHeader.actors[0], pairHeader.actors[1]);
        }
    }
}

void MyContactCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
}

void MyContactCallback::onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count)
{
}
