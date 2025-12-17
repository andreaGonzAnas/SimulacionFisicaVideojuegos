#pragma once
#include "Scene.h"
#include <vector>

class RenderItem;

struct Trapecio
{
	PxRigidStatic* palo1 = nullptr;
	PxRigidDynamic* palo2 = nullptr;
	PxRevoluteJoint* joint = nullptr;
	float motorVel = 0.0f;
	bool active = false;
};


class SceneTrapecios : public Scene
{
private:
	// SISTEMAS


	// DECO
	std::vector<RenderItem*> _scenary;

	// CAMERA
	physx::PxVec3 _initPosCamera;
	physx::PxVec3 _initDirCamera;

	// UPDATE
	PxRevoluteJoint* art;
	float initialMotorVel = 1.0f;

	// TRAPECIOS
	std::vector<Trapecio> _trapecios;


public:
	SceneTrapecios(PxPhysics* physics, PxScene* scene);
	~SceneTrapecios();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;

	bool handleKey(unsigned char key, const PxTransform& camera);

	void createDeco();
	void createTrapecio(physx::PxVec3 pos, bool startActive = true);
};

