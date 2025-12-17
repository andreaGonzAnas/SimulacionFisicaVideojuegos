#pragma once
#include "Scene.h"


class MalabaresScene : public Scene
{
private:
	PxRigidDynamic* _leftHand;
	PxRigidDynamic* _rightHand;

	// CAMERA
	physx::PxVec3 _initPosCamera;
	physx::PxVec3 _initDirCamera;

	// MANOS
	float _leftHandAngleZ = 0.0f;   // en radianes
	float _rightHandAngleZ = 0.0f;
	const float MAX_ANGLE = PxPi / 4; // 45 grados
	const float MIN_ANGLE = -PxPi / 4;
	const float DELTA_ANGLE = PxPi / 180 * 5; // 5 grados por pulsación



public:
	MalabaresScene(PxPhysics* physics, PxScene* scene);
	~MalabaresScene();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;
	bool handleKey(unsigned char key, const PxTransform& camera);

	PxRigidDynamic* createHand(physx::PxVec3 pos);
};

