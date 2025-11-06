#pragma once
#include "Scene.h"

class FireParticleSystem;
class FireworksParticleSystem;
class ConfettiParticleSystem;
class ProyectilSystem;

class Scene1 : public Scene
{
private:
	//SISTEMAS
	FireParticleSystem* _firePartSystem;
	FireworksParticleSystem* _fireworkPartSys;
	ConfettiParticleSystem* _confettiPartSys;
	ProyectilSystem* _proyectilSys;

	//FUERZAS
	bool _gravityOn = true;
	bool _windOn = true;
	bool _windWhirlOn = true;

public:
	// Constructora y destructora
	Scene1(PxPhysics* physics);
	~Scene1();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;

	bool handleKey(unsigned char key, const PxTransform& camera);
};

