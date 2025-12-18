#include "InitialMenuScene.h"
#include "MyContactCallback.h"
#include "PxShape.h"
#include "RenderUtils.hpp"

#include <PxPhysicsAPI.h>

InitialMenuScene::InitialMenuScene(PxPhysics* physics, PxScene* scene) : Scene(physics)
{
    set_gScene(scene);
}

InitialMenuScene::~InitialMenuScene()
{
}

void InitialMenuScene::init()
{
    // texto

    // Callback
    _myCallback = new MyContactCallback(nullptr, this);


    //camera
    // ---- CAMERA ----
    PxVec3 centro(35, 40, 35);
    Camera* cam = GetCamera();
    _initPosCamera = cam->getEye();
    _initDirCamera = cam->getDir();
    cam->setTransform(PxVec3(centro.x, centro.y - 10.0, centro.z - 45.0));
    cam->setDir(PxVec3(0.0, 0.0, 1));
    cam->setHumanCannonMode(true);

    // estanterias
    createEstanteria(PxVec3(20, 20, 20));
    createEstanteria(PxVec3(50, 20, 20));

    // cubos
    createCubes(PxVec3(20, 25, 20));
    createCubes(PxVec3(50, 25, 20));

    // sistema proyectiles rigidos


}

void InitialMenuScene::update(double t)
{
}

void InitialMenuScene::clear()
{
}

bool InitialMenuScene::handleKey(unsigned char key, const PxTransform& camera)
{
    return false;
}

void InitialMenuScene::handleContact(PxRigidActor* a, PxRigidActor* b)
{

}

void InitialMenuScene::createEstanteria(physx::PxVec3 pos)
{
    // Crear actor dinámico
    PxRigidStatic* hand = gPhysics->createRigidStatic(PxTransform(pos));

    // Crear forma (caja o esfera)
    PxShape* handShape = CreateShape(PxBoxGeometry(5, 0.5, 2));
    hand->attachShape(*handShape);

    // Agregar al escenario
    _gScene->addActor(*hand);

    // Crear RenderItem para visualizar la mano
    RenderItem* rHand = new RenderItem(handShape, hand, Vector4(1.0f, 0.85f, 0.7f, 1.0f)); // color piel aproximado
    
}

void InitialMenuScene::createCubes(physx::PxVec3 pos)
{
    // Crear actor dinámico
    PxRigidDynamic* hand = gPhysics->createRigidDynamic(PxTransform(pos));

    // Crear forma (caja o esfera)
    physx::PxShape* handShape = CreateShape(PxBoxGeometry(2, 2, 2));
    hand->attachShape(*handShape);

    PxRigidBodyExt::updateMassAndInertia(*hand, 20.0);

    // Agregar al escenario
    _gScene->addActor(*hand);

    // Crear RenderItem para visualizar la mano
    RenderItem* rHand = new RenderItem(handShape, hand, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
   
}
