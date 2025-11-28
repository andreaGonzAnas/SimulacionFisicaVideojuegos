#include "BuoyancyParticleSystem.h"
#include "BuoyancyForceGenerator.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"


BuoyancyParticleSystem::BuoyancyParticleSystem(PxMaterial* gMaterial)
{
	//crear registro fuerzas
	_registry = new ParticleForceRegistry();

	// --- CREACION PARTICULAS ---

	// Particula liquido
	Particula* pLiquid = new Particula({ 0.0, 40.0,0.0 }, { 0.0, 0.0,0.0 }, 0, 0.0);
	PxBoxGeometry gCube = PxBoxGeometry(10.0f, 2.0f, 10.0f);
	physx::PxShape* cubeShape = CreateShape(gCube, gMaterial);
	RenderItem* renderItem = new RenderItem(cubeShape, pLiquid->getTr(), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	pLiquid->setRenderItem(renderItem);

	// Particula obj de flotacion
	Particula* pFlot = new Particula({ 0.0,60.0,0.0 }, { 0.0, 0.0,0.0 }, 0.99, 20.0);
	PxBoxGeometry gCubeFlot = PxBoxGeometry(2.0f, 2.0f, 2.0f);
	physx::PxShape* cubeShapeFlot = CreateShape(gCubeFlot, gMaterial);
	RenderItem* renderItemFlot = new RenderItem(cubeShapeFlot, pFlot->getTr(), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	pFlot->setRenderItem(renderItemFlot);

	// FUERZA DE MUELLE
	// h v d liquidP
	BuoyancyForceGenerator* f = new BuoyancyForceGenerator(2.0f, 10.0f, 1000, pLiquid); // estatica (p2) + p.mov (p3)
	f->setActive(true);

	//añadir al registro
	_registry->add(pFlot, f);

	gravityEarth = new GravityForceGenerator();
	gravityEarth->setActive(true);
	_registry->add(pFlot, gravityEarth);

	_particles.push_back(pLiquid);
	_particles.push_back(pFlot);

}

BuoyancyParticleSystem::~BuoyancyParticleSystem()
{
}

void BuoyancyParticleSystem::update(double t)
{
	//actualizar fuerzas
	_registry->updateForces(t);

	// Actualizar todas las existentes
	for (auto p : _particles)
	{
		p->integrate(t);

	}
}
