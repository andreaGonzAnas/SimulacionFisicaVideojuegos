#pragma once
#include "Scene.h"
#include <vector>

class ProyectilSystem;
class FireParticleSystem;
class FireworksParticleSystem;
class ConfettiParticleSystem;
class RenderItem;

class Scene2 : public Scene
{
private:
	// SISTEMAS
	ProyectilSystem* _proyectilSys;
	std::vector<FireParticleSystem*> _firesInScene; //para hacer el circulo
	FireworksParticleSystem* _firework;
	std::vector<ConfettiParticleSystem*> _confettis;
	std::vector<RenderItem*> _scenary;

	bool _hasPassedFire;
	physx::PxVec3 _initPosCamera;
	physx::PxVec3 _initDirCamera;

	bool _gravityOn = true;
	bool _windOn = true;
	bool _windWhirlOn = false;

public:
	Scene2(PxPhysics* physics);
	~Scene2();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;

	bool handleKey(unsigned char key, const PxTransform& camera);

	void startCelebration();
	void createNewFirework();
	void createWalls();
};

