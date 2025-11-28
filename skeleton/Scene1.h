#pragma once
#include "Scene.h"
#include "Particula.h"
#include <vector>

class SpringParticleSystem;
class BuoyancyParticleSystem;

class Scene1 : public Scene
{
private:
	std::vector<RenderItem*> _parts;

	SpringParticleSystem* _springSys;
	BuoyancyParticleSystem* _flotationSys;
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

