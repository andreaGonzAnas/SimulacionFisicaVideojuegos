#pragma once
#include "Scene.h"
#include <vector>

class MyContactCallback;
class ProyectilRigidBodySystem;

class InitialMenuScene : public Scene
{
private:
	// COLISIONES
	MyContactCallback* _myCallback = nullptr;

	// CAMERA
	physx::PxVec3 _initPosCamera;
	physx::PxVec3 _initDirCamera;

	ProyectilRigidBodySystem* prSys;

	PxRigidDynamic* _play;
	PxRigidDynamic* _exit;

	bool changeToLevel = false;

public:
	InitialMenuScene(PxPhysics* physics, PxScene* scene);
	~InitialMenuScene();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;
	bool handleKey(unsigned char key, const PxTransform& camera);

	void handleContact(PxRigidActor* a, PxRigidActor* b);

	void createEstanteria(physx::PxVec3 pos);
	PxRigidDynamic* createCubes(physx::PxVec3 pos);

	bool getChangeLevel() { return changeToLevel; }
	SceneType getType() override { return MENU; }
};

