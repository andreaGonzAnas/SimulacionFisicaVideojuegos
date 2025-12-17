#include "MalabaresScene.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include <iostream>

MalabaresScene::MalabaresScene(PxPhysics* physics, PxScene* scene) : Scene(physics)
{
    set_gScene(scene);
}

MalabaresScene::~MalabaresScene()
{
}

void MalabaresScene::init()
{
    // ---- DECORACION ----
    //createDeco();

    // ---- CAMERA ----
    PxVec3 centro(35, 40, 35);
    Camera* cam = GetCamera();
    _initPosCamera = cam->getEye();
    _initDirCamera = cam->getDir();
    cam->setTransform(PxVec3(centro.x, centro.y - 10.0, centro.z - 45.0));
    cam->setDir(PxVec3(0.0, 0.0, 1));
    cam->setHumanCannonMode(true);

    // ---- HANDS ----
    _leftHand = createHand(PxVec3(10, 20, 35));
    _rightHand = createHand(PxVec3(60, 20, 35));


}

void MalabaresScene::update(double t)
{
}

void MalabaresScene::clear()
{
}

bool MalabaresScene::handleKey(unsigned char key, const PxTransform& camera)
{
    return false;
}

PxRigidStatic* MalabaresScene::createHand(physx::PxVec3 pos)
{
    // SUELO
    PxRigidStatic* _suelo = gPhysics->createRigidStatic(PxTransform(pos));

    // Crear forma del suelo y asignarle el material
    physx::PxShape* shapeSuelo = CreateShape(PxBoxGeometry(5, 0.5, 2));

    PxMaterial* sueloMat = gPhysics->createMaterial(
        0.2f,  // fricción estática
        0.2f,  // fricción dinámica
        0.5f   // restitución = 0 → sin rebote
    );

    shapeSuelo->setMaterials(&sueloMat, 1);

    _suelo->attachShape(*shapeSuelo);
    _gScene->addActor(*_suelo);

    // Pintar suelo
    RenderItem* item;
    item = new RenderItem(shapeSuelo, _suelo, { 0.6f, 0.4f, 0.3f, 1.0f }
    );

    return _suelo;
}
