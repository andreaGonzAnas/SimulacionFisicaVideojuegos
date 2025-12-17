#pragma once
#include "ParticleSystem.h"

class SpringForceGenerator;

class CollectibleParticleSystem : public ParticleSystem
{
private:
	std::vector<SpringForceGenerator*> _generators;
	Particula* _staticParticle;
	Particula* _movingParticle;
public:
	CollectibleParticleSystem(PxMaterial* gMaterial, physx::PxVec3 pos);
	~CollectibleParticleSystem();

	void update(double t) override;

	Particula* getStaticPart() { return _staticParticle; }
	Particula* getMovingPart() { return _movingParticle; }

	void setStaticPos(const PxVec3& pos);
};

