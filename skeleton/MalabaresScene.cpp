#include "MalabaresScene.h"

MalabaresScene::MalabaresScene(PxPhysics* physics, PxScene* scene) : Scene(physics)
{
    set_gScene(scene);
}

MalabaresScene::~MalabaresScene()
{
}

void MalabaresScene::init()
{
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
