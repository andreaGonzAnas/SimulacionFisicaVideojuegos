#pragma once
#include "ParticleSystem.h"
class ProyectilSystem : public ParticleSystem
{
private:

public:
	ProyectilSystem();
	~ProyectilSystem();

	void update(double t) override;
	void createProyectil(Vector4 color, double size, double masaR, double velR, double velS, physx::PxShape* esferaShape);
};

