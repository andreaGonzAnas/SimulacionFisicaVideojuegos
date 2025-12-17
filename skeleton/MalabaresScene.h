#pragma once
#include "Scene.h"
class MalabaresScene : public Scene
{
private:

public:
	MalabaresScene(PxPhysics* physics, PxScene* scene);
	~MalabaresScene();

	// Metodos de escena
	void init() override;
	void update(double t) override;
	void clear() override;
	bool handleKey(unsigned char key, const PxTransform& camera);


};

