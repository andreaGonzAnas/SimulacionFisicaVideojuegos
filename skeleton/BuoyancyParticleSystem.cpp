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
	Particula* pLiquid = new Particula({ 0.0, 39.0,0.0 }, { 0.0, 0.0,0.0 }, 0, 0.0);
	PxBoxGeometry gCube = PxBoxGeometry(10.0f, 1.0f, 10.0f);
	physx::PxShape* cubeShape = CreateShape(gCube, gMaterial);
	RenderItem* renderItem = new RenderItem(cubeShape, pLiquid->getTr(), Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	pLiquid->setRenderItem(renderItem);

	// Particula obj de flotacion
	pFlot = new Particula({ 0.0,45.0,0.0 }, { 0.0, 1.0,0.0 }, 0.98, 500);
	PxBoxGeometry gCubeFlot = PxBoxGeometry(2.0f, 2.0f, 2.0f);
	physx::PxShape* cubeShapeFlot = CreateShape(gCubeFlot, gMaterial);
	RenderItem* renderItemFlot = new RenderItem(cubeShapeFlot, pFlot->getTr(), Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	pFlot->setRenderItem(renderItemFlot);
	pFlot->setTimeVida(70000);

	// GRAVEDAD
	gravityEarth = new GravityForceGenerator();
	gravityEarth->setActive(true);
	_registry->add(pFlot, gravityEarth);

	_particles.push_back(pLiquid);
	_particles.push_back(pFlot);


	// FUERZA DE MUELLE
	float densidadAgua = 1000.0f;
	float volumenCubo = 1.0f;    // 2x2x2
	float alturaRealObjeto = 0.3f;

	f = new BuoyancyForceGenerator(
		alturaRealObjeto, volumenCubo, densidadAgua, pLiquid
	);
	
	
	f->setActive(true);

	//añadir al registro
	_registry->add(pFlot, f);

	

}

BuoyancyParticleSystem::~BuoyancyParticleSystem()
{
	for (auto p : _particles)
	{
		if (p->getRenderItem()) {
			DeregisterRenderItem(p->getRenderItem());
			delete p->getRenderItem();
		}
		delete p;
	}
	_particles.clear();
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

void BuoyancyParticleSystem::setMasa(double cant)
{
	pFlot->setMasa(pFlot->getMasa() + cant);
}

void BuoyancyParticleSystem::setVolume(double cant)
{
	f->setVolume(f->getVolume() + cant);
}
