#pragma once
#include "Scene.h"
#include "Particula.h"
#include <vector>

class Scene3 : public Scene
{
private:
	std::vector<RenderItem*> _parts;

public:
	Scene3(PxPhysics* physics);
	~Scene3();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;

	bool handleKey(unsigned char key, const PxTransform& camera);
};

