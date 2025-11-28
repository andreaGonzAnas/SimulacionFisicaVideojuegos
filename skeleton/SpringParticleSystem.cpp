#include "SpringParticleSystem.h"
#include "AnchoredSpringFG.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"


SpringParticleSystem::SpringParticleSystem(PxMaterial* gMaterial)
{
	// Particula con algo fijo:
	// 
	//crear registro fuerzas
	_registry = new ParticleForceRegistry();

	// Particula 3 que se mueve
	Particula* p3 = new Particula({ 0.0,20.0,0.0 }, { 0.0, 5.0,0.0 }, 0.99, 100);
	PxSphereGeometry gSphere = PxSphereGeometry();
	gSphere.radius = 0.5;
	physx::PxShape* esferaShape = CreateShape(gSphere, gMaterial);

	RenderItem* renderItem = new RenderItem(esferaShape, p3->getTr(), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	p3->setRenderItem(renderItem);
	p3->setTimeVida(70000);
	

	Particula* p4 = new Particula({ -5.0,20.0,0.0 }, { 0.0, 0.0,0.0 }, 0, 0.0);
	gSphere = PxSphereGeometry();
	gSphere.radius = 0.5;
	esferaShape = CreateShape(gSphere, gMaterial);

	RenderItem* renderItem4 = new RenderItem(esferaShape, p4->getTr(), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	p4->setRenderItem(renderItem4);


	SpringForceGenerator* f3 = new SpringForceGenerator(500, 1, p4);

	//añadir al registro
	_registry->add(p3, f3);
	_particles.push_back(p3);
	_particles.push_back(p4);

	//añadir gravedad
	gravityEarth = new GravityForceGenerator();

	_registry->add(p3, gravityEarth);
	f3->setActive(true);


	// 2 particulas con muelle:
	// 

	// Particula 0 que se mueve
	Particula* p0 = new Particula({ 20.0,20.0,0.0 }, { 0.0, 0.0,0.0 }, 0.99, 100);
	gSphere = PxSphereGeometry();
	gSphere.radius = 0.5;
	esferaShape = CreateShape(gSphere, gMaterial);

	RenderItem* renderItem0 = new RenderItem(esferaShape, p0->getTr(), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	p0->setRenderItem(renderItem);
	p0->setTimeVida(70000);

	// Particula 1 que se mueve
	Particula* p1 = new Particula({ 30.0,20.0,0.0 }, { 0.0, 0.0,0.0 }, 0.99, 100);
	gSphere = PxSphereGeometry();
	gSphere.radius = 0.5;
	esferaShape = CreateShape(gSphere, gMaterial);

	RenderItem* renderItem1 = new RenderItem(esferaShape, p1->getTr(), Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	p1->setRenderItem(renderItem);
	p1->setTimeVida(70000);


	SpringForceGenerator* f0 = new SpringForceGenerator(500, 1, p1);
	SpringForceGenerator* f1 = new SpringForceGenerator(500, 1, p0);

	//añadir al registro
	_registry->add(p0, f0);
	_registry->add(p1, f1);
	_particles.push_back(p0);
	_particles.push_back(p1);

	//añadir gravedad
	//gravityEarth = new GravityForceGenerator();
	//_registry->add(p0, gravityEarth);
	//_registry->add(p1, gravityEarth);
	f0->setActive(true);
}

SpringParticleSystem::~SpringParticleSystem()
{
}

void SpringParticleSystem::update(double t)
{
	//actualizar fuerzas
	_registry->updateForces(t);

	// Actualizar todas las existentes
	for (auto p : _particles)
	{
		p->integrate(t);

		/*
		std::cout << "Pos: " << p->getPos().x << ", "
			<< p->getPos().y << ", "
			<< p->getPos().z << std::endl;*/
	}

	//no eliminar particula
}
