#include "InitialMenuScene.h"
#include "MyContactCallback.h"
#include "ProyectilRigidBodySystem.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include <iostream>

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
    _exit = createCubes(PxVec3(20, 25, 20));
    _play = createCubes(PxVec3(50, 25, 20));
    
    // sistema proyectiles rigidos
    prSys = new ProyectilRigidBodySystem(gPhysics, _gScene);

}

void InitialMenuScene::update(double t)
{
    prSys->update(t);
}

void InitialMenuScene::clear()
{
}

bool InitialMenuScene::handleKey(unsigned char key, const PxTransform& camera)
{
    switch (key)
    {
        case ' ': // Disparar
        {
            PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
            PxShape* bolaShape = gPhysics->createShape(PxSphereGeometry(1.0f), *gMaterial);

            bolaShape->setSimulationFilterData(PxFilterData(1, 1, 1, 1));
            bolaShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, true);
            bolaShape->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, true);

            // Obtenemos posición y dirección de la cámara
            Vector3 pos = GetCamera()->getTransform().p;
            Vector3 dir = GetCamera()->getDir() * 50.0f;

            // Creamos el proyectil
            prSys->createProyectil(gPhysics, _gScene, bolaShape, pos, dir, 0.5, 10.0, Vector4(1.0f, 0.0f, 0.0f, 1.0f));

            break;
        }

        default: return false;
    }
    return true;
}

void InitialMenuScene::handleContact(PxRigidActor* a, PxRigidActor* b)
{
    // 1. Identificamos quién es quién. 
    // Necesitamos saber si uno de los dos es un proyectil.
    bool involucraProyectil = false;

    // Recorremos los proyectiles activos para ver si 'a' o 'b' es uno de ellos
    for (auto p : prSys->getRigidBodies()) { // Necesitarás un getter en prSys que devuelva la lista
        if (a == p->getRigidDynamic()|| b == p->getRigidDynamic()) {
            involucraProyectil = true;
            break;
        }
    }

    // 2. Solo ejecutamos la lógica si un proyectil está involucrado
    if (involucraProyectil)
    {
        if (a == _play || b == _play)
        {
            std::cout << "¡PLAY! Iniciando nivel..." << std::endl;
        }
        else if (a == _exit || b == _exit)
        {
            std::cout << "¡EXIT! Saliendo..." << std::endl;
            exit(0);
        }
    }
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

PxRigidDynamic* InitialMenuScene::createCubes(physx::PxVec3 pos)
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
    return hand;
}
