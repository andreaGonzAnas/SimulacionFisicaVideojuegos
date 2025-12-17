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
    _rightHand = createHand(PxVec3(15, 20, 20));
    _leftHand = createHand(PxVec3(55, 20, 20));
    

}

void MalabaresScene::update(double t)
{

}

void MalabaresScene::clear()
{
}

bool MalabaresScene::handleKey(unsigned char key, const PxTransform& camera)
{
    PxTransform currentPoseL = _leftHand->getGlobalPose();
    PxTransform currentPoseR = _rightHand->getGlobalPose();

    switch (key)
    {
        // MANO IZQUIERDA MOVIMIENTO
        case 'h': currentPoseL.p.x -= 0.5f; break;
        case 'f': currentPoseL.p.x += 0.5f; break;

            // MANO DERECHA MOVIMIENTO
        case 'j': currentPoseR.p.x -= 0.5f; break;
        case 'l': currentPoseR.p.x += 0.5f; break;

            // MANO IZQUIERDA ROTACION Z
        case 't': // girar positivo
            _leftHandAngleZ += DELTA_ANGLE;
            if (_leftHandAngleZ > MAX_ANGLE) _leftHandAngleZ = MAX_ANGLE;
            break;
        case 'g': // girar negativo
            _leftHandAngleZ -= DELTA_ANGLE;
            if (_leftHandAngleZ < MIN_ANGLE) _leftHandAngleZ = MIN_ANGLE;
            break;

            // MANO DERECHA ROTACION Z
        case 'i': _rightHandAngleZ += DELTA_ANGLE;
            if (_rightHandAngleZ > MAX_ANGLE) _rightHandAngleZ = MAX_ANGLE;
            break;
        case 'k': _rightHandAngleZ -= DELTA_ANGLE;
            if (_rightHandAngleZ < MIN_ANGLE) _rightHandAngleZ = MIN_ANGLE;
            break;

        default: return false;
    }

    // Aplicar rotaciones
    PxQuat rotL(_leftHandAngleZ, PxVec3(0, 0, 1));
    currentPoseL.q = rotL;
    _leftHand->setKinematicTarget(currentPoseL);

    PxQuat rotR(_rightHandAngleZ, PxVec3(0, 0, 1));
    currentPoseR.q = rotR;
    _rightHand->setKinematicTarget(currentPoseR);

    return true;
}

PxRigidDynamic* MalabaresScene::createHand(physx::PxVec3 pos)
{
    // Crear actor dinámico
    PxRigidDynamic* hand = gPhysics->createRigidDynamic(PxTransform(pos));

    // Crear forma (caja o esfera)
    PxShape* handShape = CreateShape(PxBoxGeometry(5, 0.5, 2));
    hand->attachShape(*handShape);

    // Poner la mano como kinemática para control manual
    hand->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);

    // Agregar al escenario
    _gScene->addActor(*hand);

    // Crear RenderItem para visualizar la mano
    RenderItem* rHand = new RenderItem(handShape, hand, Vector4(1.0f, 0.85f, 0.7f, 1.0f)); // color piel aproximado
    RegisterRenderItem(rHand);

    return hand;
}
