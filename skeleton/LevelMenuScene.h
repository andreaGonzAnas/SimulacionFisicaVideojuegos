#pragma once
#include "Scene.h"
#include <vector>

class MyContactCallback;
class ProyectilRigidBodySystem;
class RenderItem;

class LevelMenuScene : public Scene
{
private:
	// COLISIONES
	MyContactCallback* _myCallback = nullptr;

	// CAMERA
	physx::PxVec3 _initPosCamera;
	physx::PxVec3 _initDirCamera;

	ProyectilRigidBodySystem* prSys;

	PxRigidDynamic* _balas;
	PxRigidDynamic* _trapecios;
	PxRigidDynamic* _back;

	int changeToLevel = -1;

	std::vector<RenderItem*> _scenary;
	std::vector<physx::PxRigidDynamic*> _rigids;
	std::vector<PxRigidStatic*> _statics;


public:
	LevelMenuScene(PxPhysics* physics, PxScene* scene);
	~LevelMenuScene();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;
	bool handleKey(unsigned char key, const PxTransform& camera);

	void handleContact(PxRigidActor* a, PxRigidActor* b);

	void createEstanteria(physx::PxVec3 pos);
	PxRigidDynamic* createCubes(physx::PxVec3 pos);

	int getButtonResult() { return changeToLevel; }
	SceneType getType() override { return OPTIONS; }

};

