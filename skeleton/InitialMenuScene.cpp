#include "InitialMenuScene.h"

InitialMenuScene::InitialMenuScene(PxPhysics* physics, PxScene* scene) : Scene(physics)
{
    set_gScene(scene);
}

InitialMenuScene::~InitialMenuScene()
{
}

void InitialMenuScene::init()
{
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
