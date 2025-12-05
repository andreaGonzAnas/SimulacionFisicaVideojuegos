#include "SpringParticleSystem.h"
#include "AnchoredSpringFG.h"
#include "ParticleForceRegistry.h"
#include "GravityForceGenerator.h"


SpringParticleSystem::SpringParticleSystem(PxMaterial* gMaterial)
{
	//crear registro fuerzas
	_registry = new ParticleForceRegistry();

	// --- PARTICULA CON ESTATICO + SLINKY ---

	// Particula estatica
	Particula* p2 = new Particula({ -5.0,20.0,0.0 }, { 0.0, 0.0,0.0 }, 0, 0.0);
	PxSphereGeometry gSphere = PxSphereGeometry();
	gSphere.radius = 0.5;
	physx::PxShape* esferaShape = CreateShape(gSphere, gMaterial);

	RenderItem* renderItem2 = new RenderItem(esferaShape, p2->getTr(), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	p2->setRenderItem(renderItem2);

	// Particula 3 que se mueve
	Particula* p3 = new Particula({ 0.0,20.0,0.0 }, { 0.0, 5.0,0.0 }, 0.99, 100);
	gSphere = PxSphereGeometry();
	gSphere.radius = 0.5;
	esferaShape = CreateShape(gSphere, gMaterial);

	RenderItem* renderItem = new RenderItem(esferaShape, p3->getTr(), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	p3->setRenderItem(renderItem);
	p3->setTimeVida(70000);

	// Particula 2 que se mueve
	Particula* p4 = new Particula({ 5.0,20.0,0.0 }, { 0.0, 5.0,0.0 }, 0.99, 100);
	gSphere = PxSphereGeometry();
	gSphere.radius = 0.5;
	esferaShape = CreateShape(gSphere, gMaterial);

	RenderItem* renderItem4 = new RenderItem(esferaShape, p4->getTr(), Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	p4->setRenderItem(renderItem4);
	p4->setTimeVida(70000);
	
	
	// FUERZA DE MUELLE
	SpringForceGenerator* f3 = new SpringForceGenerator(50, 1, p2); // estatica (p2) + p.mov (p3)
	SpringForceGenerator* f4 = new SpringForceGenerator(50, 1, p3); // p.mov (p3) + p.mov (p4)
	f3->setActive(true);
	f4->setActive(true);
	_vFuerzas.push_back(f3);
	_vFuerzas.push_back(f4);

	//añadir al registro
	_registry->add(p3, f3);
	_registry->add(p4, f4);
	_particles.push_back(p3);
	_particles.push_back(p4);

	//añadir gravedad
	gravityEarth = new GravityForceGenerator();

	_registry->add(p3, gravityEarth);
	_registry->add(p4, gravityEarth);
	

	// --- 2 PARTICULAS CON MUELLE ---

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
	p1->setRenderItem(renderItem1);
	p1->setTimeVida(70000);

	// FUERZA DE MUELLE
	SpringForceGenerator* f0 = new SpringForceGenerator(50, 1, p1);
	SpringForceGenerator* f1 = new SpringForceGenerator(50, 1, p0);
	f0->setActive(true);
	f1->setActive(true);
	_vFuerzas.push_back(f0);
	_vFuerzas.push_back(f1);

	//añadir al registro
	_registry->add(p0, f0);
	_registry->add(p1, f1);
	_particles.push_back(p0);
	_particles.push_back(p1);

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
	}
}

void SpringParticleSystem::setK(double cant)
{
	// cambiar el k de todos los sistemas de muelles
	for (auto& f : _vFuerzas)
	{
		f->setK(f->getK() + cant);
	}
}
