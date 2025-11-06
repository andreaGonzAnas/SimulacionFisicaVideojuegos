#pragma once
#include "Scene.h"
#include <vector>

class ProyectilSystem;
class FireParticleSystem;

class Scene2 : public Scene
{
private:
	ProyectilSystem* _proyectilSys;
	std::vector<FireParticleSystem*> _firesInScene; //para hacer el circulo

public:
	Scene2(PxPhysics* physics);
	~Scene2();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;

	bool handleKey(unsigned char key, const PxTransform& camera);
};

