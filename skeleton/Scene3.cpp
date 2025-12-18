#include "Scene3.h"
#include "PxShape.h"
#include "RenderUtils.hpp"
#include "Vector3D.h"
#include "ExplosionRigidBodySystem.h"

#include <PxPhysicsAPI.h>

Scene3::Scene3(PxPhysics* physics, PxScene* scene): Scene(physics)
{
	set_gScene(scene);
}

Scene3::~Scene3()
{
    //clear();
}

void Scene3::init()
{
	// SUELO
	_suelo = gPhysics->createRigidStatic(PxTransform({ 50, 0, -80 }));
	
	// Crear material del suelo: poca fricción, muy elástico
	physx::PxMaterial* sueloMat = gPhysics->createMaterial(0.0f, 0.0f, 0.9f); // friction: 0, restitution: 0.9

	// Crear forma del suelo y asignarle el material
	physx::PxShape* shapeSuelo = CreateShape(PxBoxGeometry(100, 0.1, 100));
	shapeSuelo->setMaterials(&sueloMat, 1);

	//physx::PxShape* shapeSuelo = CreateShape(PxBoxGeometry(100, 0.1, 100));
	_suelo->attachShape(*shapeSuelo);
	_gScene->addActor(*_suelo);

	// Pintar suelo
	RenderItem* item;
	item = new RenderItem(shapeSuelo, _suelo, { 0.8, 0.8,0.8,1 });

	_parts.push_back(item);

	
	// Anadir un actor dinamico
	PxRigidDynamic* new_solid;
	new_solid = gPhysics->createRigidDynamic(PxTransform({ 50,200,-80 }));
	new_solid->setLinearVelocity({ 0,5,0 });
	new_solid->setAngularVelocity({ 0,0,0 });
	physx::PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));
	new_solid->attachShape(*shape_ad);

	PxRigidBodyExt::updateMassAndInertia(*new_solid, 0.15);
	
	_expSys = new ExplosionRigidBodySystem(new_solid, gPhysics, _gScene);
	
}

void Scene3::update(double t)
{
	_expSys->update(t);
}

void Scene3::clear()
{
	// Eliminar eje de coordenadas
	for (auto p : _parts)
	{
		DeregisterRenderItem(p);
	}

	// Eliminar suelo
	if (_suelo) {
		_gScene->removeActor(*_suelo);
	}

	if (_suelo) {
		PxU32 nShapes = _suelo->getNbShapes();
		PxShape* shapes[8];
		_suelo->getShapes(shapes, nShapes);

		for (PxU32 i = 0; i < nShapes; i++) {
			shapes[i]->release();
		}
	}

	// 3. Liberar el actor estático
	if (_suelo) {
		_suelo->release();
		_suelo = nullptr;
	}

	
	delete _expSys; _expSys = nullptr;

}

bool Scene3::handleKey(unsigned char key, const PxTransform& camera)
{
    return false;
}
