#include "LevelMenuScene.h"
#include "MyContactCallback.h"
#include "ProyectilRigidBodySystem.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include <iostream>

#include <PxPhysicsAPI.h>

extern bool _levelMenu;

LevelMenuScene::LevelMenuScene(PxPhysics* physics, PxScene* scene) : Scene(physics)
{
    set_gScene(scene);
}

LevelMenuScene::~LevelMenuScene()
{
}

void LevelMenuScene::init()
{
    // texto
    _levelMenu = true;

    // Callback
    _myCallback = new MyContactCallback(nullptr, nullptr, this);

    // Camera
    PxVec3 centro(35, 40, 35);
    Camera* cam = GetCamera();
    _initPosCamera = cam->getEye();
    _initDirCamera = cam->getDir();
    cam->setTransform(PxVec3(centro.x, centro.y - 10.0, centro.z - 45.0));
    cam->setDir(PxVec3(0.0, 0.0, 1));
    cam->setHumanCannonMode(true);

    // estanterias
    createEstanteria(PxVec3(20, 25, 20));
    createEstanteria(PxVec3(50, 25, 20));

    // cubos
    _trapecios = createCubes(PxVec3(20, 30, 20));
    _balas = createCubes(PxVec3(50, 30, 20));

    // sistema proyectiles rigidos
    prSys = new ProyectilRigidBodySystem(gPhysics, _gScene);



}

void LevelMenuScene::update(double t)
{
    prSys->update(t);
}

void LevelMenuScene::clear()
{
    _initialMenu = false;

    // proyectiles
    if (prSys) { delete prSys; prSys = nullptr; }

    // vacio visual
    for (auto item : _scenary) {
        if (item) {
            DeregisterRenderItem(item);
            const_cast<RenderItem*>(item)->actor = nullptr;
        }
    }
    _scenary.clear();

    // rigidos
    for (auto r : _rigids) {
        if (r) {
            _gScene->removeActor(*r);
            PxU32 nShapes = r->getNbShapes();
            physx::PxShape* shapes[8];
            r->getShapes(shapes, nShapes);
            for (PxU32 i = 0; i < nShapes; i++) shapes[i]->release();

            if (r->userData) {
                r->userData = nullptr;
            }
            r->release();
        }
    }
    _rigids.clear();

    // estaticos
    for (auto s : _statics) {
        if (s) {
            _gScene->removeActor(*s);

            PxU32 nShapes = s->getNbShapes();
            physx::PxShape* shapes[8];
            s->getShapes(shapes, nShapes);

            for (PxU32 i = 0; i < nShapes; i++) {
                shapes[i]->release();
            }

            s->release();
            s = nullptr;
        }
    }
    _statics.clear();
}

bool LevelMenuScene::handleKey(unsigned char key, const PxTransform& camera)
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

void LevelMenuScene::handleContact(PxRigidActor* a, PxRigidActor* b)
{
    // 1. Identificamos quién es quién. 
    // Necesitamos saber si uno de los dos es un proyectil.
    bool involucraProyectil = false;

    // Recorremos los proyectiles activos para ver si 'a' o 'b' es uno de ellos
    for (auto p : prSys->getRigidBodies()) { // Necesitarás un getter en prSys que devuelva la lista
        if (a == p->getRigidDynamic() || b == p->getRigidDynamic()) {
            involucraProyectil = true;
            break;
        }
    }

    // 2. Solo ejecutamos la lógica si un proyectil está involucrado
    if (involucraProyectil)
    {
        if (a == _balas || b == _balas)
        {
            std::cout << "a las balas" << std::endl;
            changeToLevel = 1;
        }
        else if (a == _trapecios || b == _trapecios)
        {
            std::cout << "a los trapecios" << std::endl;
            changeToLevel = 2;
        }
    }
}

void LevelMenuScene::createEstanteria(physx::PxVec3 pos)
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
    _scenary.push_back(rHand);
    _statics.push_back(hand);
}

PxRigidDynamic* LevelMenuScene::createCubes(physx::PxVec3 pos)
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
    _scenary.push_back(rHand);
    _rigids.push_back(hand);

    return hand;
}
