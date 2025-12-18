#pragma once
#include "Scene.h"

class InitialMenuScene : public Scene
{
private:


public:
	InitialMenuScene(PxPhysics* physics, PxScene* scene);
	~InitialMenuScene();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;
	bool handleKey(unsigned char key, const PxTransform& camera);

};

