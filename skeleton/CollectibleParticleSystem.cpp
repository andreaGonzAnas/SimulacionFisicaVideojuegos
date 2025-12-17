#include "CollectibleParticleSystem.h"
#include "AnchoredSpringFG.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"

CollectibleParticleSystem::CollectibleParticleSystem(PxMaterial* gMaterial, physx::PxVec3 pos)
{
	//crear registro fuerzas
	_registry = new ParticleForceRegistry();

	// --- PARTICULA CON ESTATICO + SLINKY ---

	// Particula estatica
	_staticParticle = new Particula(pos, { 0.0, 0.0,0.0 }, 0, 0.0);
	PxSphereGeometry gSphere = PxSphereGeometry();
	gSphere.radius = 0.5;
	physx::PxShape* esferaShape = CreateShape(gSphere, gMaterial);

	RenderItem* renderItem2 = new RenderItem(esferaShape, _staticParticle->getTr(), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	_staticParticle->setRenderItem(renderItem2);

	// Particula 3 que se mueve
	_movingParticle = new Particula(pos + PxVec3(0.0,-1.0,0.0), {0.5, 0.0,0.0}, 0.99, 100);
	gSphere = PxSphereGeometry();
	gSphere.radius = 0.5;
	esferaShape = CreateShape(gSphere, gMaterial);

	RenderItem* renderItem3 = new RenderItem(esferaShape, _movingParticle->getTr(), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	_movingParticle->setRenderItem(renderItem3);
	_movingParticle->setTimeVida(70000);


	// FUERZA DE MUELLE
	SpringForceGenerator* f3 = new SpringForceGenerator(200, 1, _staticParticle); // estatica (p2) + p.mov (p3)
	f3->setActive(true);
	_generators.push_back(f3);

	//añadir al registro
	_registry->add(_movingParticle, f3);
	_particles.push_back(_staticParticle);
	_particles.push_back(_movingParticle);

	//añadir gravedad
	gravityEarth = new GravityForceGenerator();
	_registry->add(_movingParticle, gravityEarth);


}

CollectibleParticleSystem::~CollectibleParticleSystem()
{
	for (auto f : _generators)
		delete f;
	_generators.clear();

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

void CollectibleParticleSystem::update(double t)
{
	// Aplicar fuerza leve si sigue activa 
	if (!_particles.empty())
	{
		_movingParticle->addForce({ 100.0, 0.0, 0.0 });
	}
	
	//actualizar fuerzas
	_registry->updateForces(t);

	// Actualizar todas las existentes
	for (auto p : _particles)
	{
		p->integrate(t);
	}

}

void CollectibleParticleSystem::setStaticPos(const PxVec3& pos)
{
	_staticParticle->setPos(pos);
	_staticParticle->setPrePos(pos);
}
