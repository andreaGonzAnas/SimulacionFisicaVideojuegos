#pragma once
#include "Scene.h"


class MalabaresScene : public Scene
{
private:
	PxRigidStatic* _leftHand;
	PxRigidStatic* _rightHand;

	// CAMERA
	physx::PxVec3 _initPosCamera;
	physx::PxVec3 _initDirCamera;



public:
	MalabaresScene(PxPhysics* physics, PxScene* scene);
	~MalabaresScene();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;
	bool handleKey(unsigned char key, const PxTransform& camera);

	PxRigidStatic* createHand(physx::PxVec3 pos);
};

