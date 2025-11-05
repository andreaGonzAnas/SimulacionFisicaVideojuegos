#pragma once
#include "Scene.h"

class Scene0 : public Scene
{
public:

	// Constructora y destructora
	Scene0(PxPhysics* physics);
	~Scene0();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;
};

